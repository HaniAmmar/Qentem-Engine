/**
 * @file QAllocator.hpp
 * @brief Core allocation and construction helpers.
 *
 * This header defines Qentem's central allocation interface, designed to
 * replace raw operator new/delete usage and prepare for future in-house
 * memory pools. It provides aligned allocation, default and parameterized
 * construction, and safe object destruction — all without dependencies.
 *
 * Intended for use across containers, buffers, sessions, and internal pools.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_ALLOCATOR_H
#define QENTEM_ALLOCATOR_H

#include "MemoryUtils.hpp"

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
// TODO: Build a real Allocator
struct QAllocator {
    ///////////////////////////////////////////////////////////
    //              Basic Allocation / Deallocation          //
    ///////////////////////////////////////////////////////////

    template <typename Type_T>
    inline static Type_T *Allocate(SizeT size) {
        return static_cast<Type_T *>(QENTEM_ALLOCATE(SystemIntType(size * sizeof(Type_T))));
    }

    template <typename Type_T>
    inline static void Deallocate(Type_T *pointer) noexcept {
        QENTEM_DEALLOCATE(pointer);
    }

    ///////////////////////////////////////////////////////////
    //             Allocate + Initialize in One Step         //
    ///////////////////////////////////////////////////////////

    template <typename Type_T>
    QENTEM_INLINE static Type_T *AllocateInit() {
        Type_T *pointer = Allocate<Type_T>(1);
        MemoryUtils::Initialize(pointer);
        return pointer;
    }

    template <typename Type_T, typename... Values_T>
    QENTEM_INLINE static Type_T *AllocateInit(Values_T &&...values) noexcept {
        Type_T *pointer = Allocate<Type_T>(1);
        MemoryUtils::Initialize(pointer, QUtility::Forward<Values_T>(values)...);
        return pointer;
    }

    ///////////////////////////////////////////////////////////
    //                Aligned Allocation (Optional)          //
    ///////////////////////////////////////////////////////////

    template <typename Type_T>
    inline static Type_T *AllocateAligned(SystemIntType count, SystemIntType alignment = alignof(Type_T)) noexcept {
        // constexpr SystemIntType min_align = alignof(void *);
        // alignment                         = (alignment < min_align) ? min_align : alignment;

        --alignment;
        SystemIntType padding = (alignment + sizeof(void *));

        void *raw = QENTEM_RAW_ALLOCATE((sizeof(Type_T) * count) + padding);

        void *aligned = reinterpret_cast<void *>((reinterpret_cast<SystemIntType>(raw) + sizeof(void *) + alignment) &
                                                 ~(alignment));

        *(reinterpret_cast<void **>(aligned) - 1) = raw;
        return static_cast<Type_T *>(aligned);
    }

    inline static void DeallocateAligned(void *ptr) noexcept {
        if (ptr != nullptr) {
            QENTEM_RAW_DEALLOCATE((static_cast<void **>(ptr))[-1]);
        }
    }
};

} // namespace Qentem

#endif // QENTEM_ALLOCATOR_H
