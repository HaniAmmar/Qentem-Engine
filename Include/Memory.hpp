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

#ifndef _MALLOC_H
#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif
#endif

#include <new>

#ifndef QENTEM_MEMORY_H_
#define QENTEM_MEMORY_H_

namespace Qentem {
namespace Memory {

static void SetToZero(void *ptr, ULong size) noexcept {
    char *      des = static_cast<char *>(ptr);
    const char *end = (des + size);

    while (des != end) {
        *des = 0;
        ++des;
    }
}

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

} // namespace Memory

class HAllocator {
  public:
    template <typename Type_>
    inline static Type_ *Allocate(ULong size) {
        return static_cast<Type_ *>(malloc(size * sizeof(Type_)));
    }

    template <typename Type_, typename... Values_T_>
    inline static Type_ *AllocateInit(Values_T_ &&... values) {
        Type_ *ptr = Allocate<Type_>(1);
        new (ptr) Type_(static_cast<Values_T_ &&>(values)...);
        return ptr;
    }

    template <typename Type_>
    inline static Type_ *AllocateInit() {
        Type_ *ptr = Allocate<Type_>(1);
        new (ptr) Type_();
        return ptr;
    }

    template <typename Type_>
    inline static Type_ *AllocatePointers(ULong size) noexcept {
        const ULong c_size = (size * sizeof(void *));
        void *      vptr   = malloc(c_size);
        Memory::SetToZero(vptr, c_size);

        return static_cast<Type_ *>(vptr);
    }

    template <typename Type_>
    inline static void Construct(Type_ *ptr, Type_ &&value) noexcept {
        new (ptr) Type_(static_cast<Type_ &&>(value));
    }

    template <typename Type_>
    inline static void Construct(Type_ *start, const Type_ *end,
                                 const Type_ &value) noexcept {
        while (start < end) {
            new (start) Type_(value);
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
