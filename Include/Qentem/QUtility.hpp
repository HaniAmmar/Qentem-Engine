/**
 * @file QUtility.hpp
 * @brief Lightweight value manipulation and sorting utilities.
 *
 * This header defines core language-level helpers for value movement, forwarding,
 * swapping, and in-place array sorting.
 *
 * - Forward<T>(x): Perfect forwarding for universal references
 * - Move(x): Emulates std::move for manual rvalue casts
 * - Swap(a, b): Value-swapping
 * - Sort<Ascend>(arr, start, end): Simple in-place quick-partition sort
 *
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 */

#ifndef QENTEM_UTILITY_H
#define QENTEM_UTILITY_H

#include "QTraits.hpp"

///////////////////////////////////////////////////////////////
//               Compiler Intrinsic Macros                   //
///////////////////////////////////////////////////////////////
// clang-format off
#ifdef QENTEM_DEBUG
    #define QENTEM_INLINE

    #ifdef _MSC_VER
        #define QENTEM_NOINLINE
        #define QENTEM_MAYBE_UNUSED
    #else
        #define QENTEM_NOINLINE __attribute__((noinline))
        #define QENTEM_MAYBE_UNUSED __attribute__((unused))
    #endif
#else
    #ifdef _MSC_VER
        #define QENTEM_NOINLINE __declspec(noinline)
        #define QENTEM_INLINE __forceinline
        #define QENTEM_MAYBE_UNUSED
    #else
        #define QENTEM_NOINLINE __attribute__((noinline))
        #define QENTEM_INLINE __attribute__((always_inline))
        #define QENTEM_MAYBE_UNUSED __attribute__((unused))
    #endif
#endif
// clang-format on

namespace Qentem {

struct QUtility {
    ///////////////////////////////////////////////////////////
    //                    Alignment                          //
    ///////////////////////////////////////////////////////////
    template <typename Number_T>
    QENTEM_INLINE static constexpr Number_T AlignUp(Number_T size, Number_T alignment) noexcept {
        return (size + (alignment - Number_T{1})) & ~(alignment - Number_T{1});
    }

    template <typename Number_T>
    QENTEM_INLINE static constexpr Number_T AlignDown(Number_T size, Number_T alignment) noexcept {
        return size & ~(alignment - Number_T{1});
    }

    template <typename Number_T>
    QENTEM_INLINE static constexpr bool IsAligned(Number_T size, Number_T alignment) noexcept {
        return (size & (alignment - Number_T{1})) == 0;
    }
    ///////////////////////////////////////////////////////////
    //                      Forward                          //
    ///////////////////////////////////////////////////////////
    template <typename Type_T>
    QENTEM_INLINE static constexpr Type_T &&Forward(typename QTraits::ReferenceType<Type_T>::Type &value) noexcept {
        return (Type_T &&)(value);
    }

    template <typename Type_T>
    QENTEM_INLINE static constexpr Type_T &&Forward(typename QTraits::ReferenceType<Type_T>::Type &&value) noexcept {
        static_assert(!QTraits::IsLValueReference<Type_T>::Value, "Forward<T>(x): Cannot forward an lvalue as rvalue.");
        return (Type_T &&)(value);
    }

    ///////////////////////////////////////////////////////////
    //                      Move                             //
    ///////////////////////////////////////////////////////////
    template <typename Type_T>
    QENTEM_INLINE static constexpr typename QTraits::ReferenceType<Type_T>::Type &&Move(Type_T &&value) noexcept {
        return (typename QTraits::ReferenceType<Type_T>::Type &&)(value);
    }
    ///////////////////////////////////////////////////////////
    //                      Swap                             //
    ///////////////////////////////////////////////////////////
    template <typename Type_T>
    QENTEM_INLINE static void Swap(Type_T &item1, Type_T &item2) noexcept {
        Type_T item = Move(item1);
        item1       = Move(item2);
        item2       = Move(item);
    }
    ///////////////////////////////////////////////////////////
    //                      Sort                             //
    ///////////////////////////////////////////////////////////
    template <bool Ascend_T, typename Type_T, typename Number_T>
    static void Sort(Type_T *arr, Number_T start, Number_T end) noexcept {
        if (start != end) {
            Type_T  &item   = arr[start];
            Number_T index  = start;
            Number_T offset = (start + Number_T{1});

            while (offset < end) {
                if (Ascend_T) {
                    if (arr[offset] < item) {
                        ++index;
                        Swap(arr[index], arr[offset]);
                    }
                } else {
                    if (arr[offset] > item) {
                        ++index;
                        Swap(arr[index], arr[offset]);
                    }
                }

                ++offset;
            }

            if (index != start) {
                Swap(arr[index], arr[start]);
            }

            Sort<Ascend_T>(arr, start, index);
            ++index;
            Sort<Ascend_T>(arr, index, end);
        }
    }
};

} // namespace Qentem

#endif // QENTEM_UTILITY_H
