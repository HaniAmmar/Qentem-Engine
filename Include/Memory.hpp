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

#include <malloc.h>
#include <new>

#ifndef QENTEM_MEMORY_H_
#define QENTEM_MEMORY_H_

namespace Qentem {
namespace Memory {

///////////////////////// Start SetToZero /////////////////////////

template <ULong S>
struct SetToZeroHelper { // ASM QWORD
    static void SetToZero(void *ptr, ULong size) noexcept {
#ifdef QENTEM_64BIT_
        size >>= 3U;

        long long *      des = static_cast<long long *>(ptr);
        const long long *end = (des + size);

        while (des != end) {
            *des = 0;
            ++des;
        }
#else
        size >>= 2U;

        int *      des = static_cast<int *>(ptr);
        const int *end = (des + size);

        while (des != end) {
            *des = 0;
            ++des;
        }
#endif
    }
};

template <>
struct SetToZeroHelper<1> { // ASM BYTE
    static void SetToZero(void *ptr, ULong size) noexcept {
        char *      des = static_cast<char *>(ptr);
        const char *end = (des + size);

        while (des != end) {
            *des = 0;
            ++des;
        }
    }
};

template <>
struct SetToZeroHelper<2> { // ASM WORD
    static void SetToZero(void *ptr, ULong size) noexcept {
        size >>= 1U;

        short *      des = static_cast<short *>(ptr);
        const short *end = (des + size);

        while (des != end) {
            *des = 0;
            ++des;
        }
    }
};

template <>
struct SetToZeroHelper<4> { // ASM DWORD
    static void SetToZero(void *ptr, ULong size) noexcept {
        size >>= 2U;

        int *      des = static_cast<int *>(ptr);
        const int *end = (des + size);

        while (des != end) {
            *des = 0;
            ++des;
        }
    }
};

template <typename Char_T_>
static void SetToZero(void *ptr, ULong size) noexcept {
    return SetToZeroHelper<sizeof(Char_T_)>::SetToZero(ptr, size);
}

///////////////////////// End SetToZero /////////////////////////

///////////////////////// Start Copy /////////////////////////

template <ULong S>
struct CopyHelper { // ASM QWORD
    static void Copy(void *to, const void *form, ULong size) noexcept {
#ifdef QENTEM_64BIT_
        size >>= 3U;

        const long long *src = static_cast<const long long *>(form);
        const long long *end = (src + size);
        long long *      des = static_cast<long long *>(to);

        while (src != end) {
            *des = *src;
            ++des;
            ++src;
        }
#else
        size >>= 2U;

        const int *src = static_cast<const int *>(form);
        const int *end = (src + size);
        int *      des = static_cast<int *>(to);

        while (src != end) {
            *des = *src;
            ++des;
            ++src;
        }
#endif
    }
};

template <>
struct CopyHelper<1> { // ASM BYTE
    static void Copy(void *to, const void *form, ULong size) noexcept {
        const char *src = static_cast<const char *>(form);
        const char *end = (src + size);
        char *      des = static_cast<char *>(to);

        while (src != end) {
            *des = *src;
            ++des;
            ++src;
        }
    }
};

template <>
struct CopyHelper<2> { // ASM WORD
    static void Copy(void *to, const void *form, ULong size) noexcept {
        size >>= 1U;

        const short *src = static_cast<const short *>(form);
        const short *end = (src + size);
        short *      des = static_cast<short *>(to);

        while (src != end) {
            *des = *src;
            ++des;
            ++src;
        }
    }
};

template <>
struct CopyHelper<4> { // ASM DWORD
    static void Copy(void *to, const void *form, ULong size) noexcept {
        size >>= 2U;

        const int *src = static_cast<const int *>(form);
        const int *end = (src + size);
        int *      des = static_cast<int *>(to);

        while (src != end) {
            *des = *src;
            ++des;
            ++src;
        }
    }
};

template <typename Char_T_>
static void Copy(void *to, const void *form, ULong size) noexcept {
    return CopyHelper<sizeof(Char_T_)>::Copy(to, form, size);
}

///////////////////////// End Copy /////////////////////////

} // namespace Memory

class HAllocator {
  public:
    template <typename Type_>
    inline static Type_ *Allocate(ULong size) {
        return static_cast<Type_ *>(malloc(size * sizeof(Type_)));
    }

    template <typename Type_>
    inline static Type_ *Allocate(Type_ &&value) {
        Type_ *ptr = Allocate<Type_>(1);
        new (ptr) Type_(static_cast<Type_ &&>(value));
        return ptr;
    }

    // template <typename Type_>
    // static Type_ *Allocate(ULong size, Type_ &&value) {
    //     Type_ *ptr = Allocate<Type_>(size);

    //     ULong i = 0;
    //     while (i != size) {
    //         *(ptr + i) = value;
    //         ++i;
    //     }

    //     return ptr;
    // // }

    // template <typename Type_>
    // inline static Type_ *AllocateClear(ULong size) noexcept {
    //     const ULong c_size = (size * sizeof(Type_));
    //     void *      vptr   = malloc(c_size);
    //     Memory::SetToZero<Type_>(vptr, c_size);

    //     return static_cast<Type_ *>(vptr);
    // }

    template <typename Type_>
    inline static Type_ *AllocatePointers(ULong size) noexcept {
        const ULong c_size = (size * sizeof(void *));
        void *      vptr   = malloc(c_size);
        Memory::SetToZero<void *>(vptr, c_size);

        return static_cast<Type_ *>(vptr);
    }

    // template <typename Type_>
    // inline static Type_ *AllocateInit() {
    //     Type_ *ptr = Allocate<Type_>(1);
    //     new (ptr) Type_();
    //     return ptr;
    // }

    // template <typename Type_>
    // static Type_ *AllocateInit(ULong size) {
    //     Type_ *ptr = Allocate<Type_>(size);

    //     ULong i = 0;
    //     while (i != size) {
    //         new (ptr + i) Type_();
    //         ++i;
    //     }

    //     return ptr;
    // }

    template <typename Type_>
    inline static void Construct(Type_ *ptr, Type_ &&value) noexcept {
        new (ptr) Type_(static_cast<Type_ &&>(value));
    }

    template <typename Type_>
    inline static void Construct(Type_ *start, const Type_ *end,
                                 Type_ &&value) noexcept {
        while (start < end) {
            new (start) Type_(static_cast<Type_ &&>(value));
            ++start;
        }
    }

    template <typename Type_>
    inline static void Destruct(Type_ *ptr) noexcept {
        ptr->~Type_();
    }

    template <typename Type_>
    inline static void Destruct(Type_ *start, const Type_ *end) noexcept {
        while (start < end) {
            start->~Type_();
            ++start;
        }
    }

    inline static void Deallocate(void *ptr) noexcept {
        if (ptr != nullptr) {
            free(ptr);
        }
    }
};

} // namespace Qentem

#endif
