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
#include "Platform.hpp"

#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

#include <new>

#ifndef QENTEM_MEMORY_H_
#define QENTEM_MEMORY_H_

namespace Qentem {
namespace Memory {

inline static void SetToZero(void *ptr, SizeT size) noexcept {
#ifdef QENTEM_SIMD_ENABLED
    const SizeT m_size    = (size >> QENTEM_SIMD_SHIFT_SIZE);
    const SizeT remaining = (size ^ (m_size << QENTEM_SIMD_SHIFT_SIZE));

    if (m_size != 0) {
        QENTEM_SIMD_VAR *      m_ptr  = static_cast<QENTEM_SIMD_VAR *>(ptr);
        const QENTEM_SIMD_VAR *end    = (m_ptr + m_size);
        const QENTEM_SIMD_VAR  m_zero = QENTEM_SIMD_SET_TO_ZERO();

        do {
            QENTEM_SIMD_STOREU(m_ptr, m_zero);
            ++m_ptr;
        } while (m_ptr != end);

        if (remaining == 0) {
            return;
        }
    }

    char *      des = (static_cast<char *>(ptr) + (size - remaining));
    const char *end = (des + remaining);
#else
    char *      des = static_cast<char *>(ptr);
    const char *end = (des + size);
#endif

    while (des != end) {
        *des = 0;
        ++des;
    }
}

inline static void Copy(void *to, const void *from, SizeT size) noexcept {
#ifdef QENTEM_SIMD_ENABLED
    const SizeT m_size    = (size >> QENTEM_SIMD_SHIFT_SIZE);
    const SizeT remaining = (size ^ (m_size << QENTEM_SIMD_SHIFT_SIZE));

    if (m_size != 0) {
        QENTEM_SIMD_VAR *      m_to = static_cast<QENTEM_SIMD_VAR *>(to);
        const QENTEM_SIMD_VAR *m_form =
            static_cast<const QENTEM_SIMD_VAR *>(from);
        const QENTEM_SIMD_VAR *end = (m_form + m_size);

        do {
            QENTEM_SIMD_STOREU(m_to, QENTEM_SIMD_LOAD(m_form));
            ++m_form;
            ++m_to;
        } while (m_form != end);

        if (remaining == 0) {
            return;
        }
    }

    const SizeT start = (size - remaining);
    const char *src   = static_cast<const char *>(from) + start;
    const char *end   = (src + remaining);
    char *      des   = static_cast<char *>(to) + start;
#else
    const char *src = static_cast<const char *>(from);
    const char *end = (src + size);
    char *      des = static_cast<char *>(to);
#endif

    while (src != end) {
        *des = *src;
        ++des;
        ++src;
    }
}

template <typename Type_>
inline static Type_ *Allocate(SizeT size) {
    return static_cast<Type_ *>(malloc(size * sizeof(Type_)));
}

template <typename Type_, typename... Values_T_>
inline static Type_ *AllocateInit(Values_T_ &&... values) {
    Type_ *ptr = Allocate<Type_>(1);
    new (ptr) Type_(static_cast<Values_T_ &&>(values)...);
    return ptr;
}

template <typename Type_, typename... Values_T_>
inline static Type_ *AllocateInit(const Values_T_ &... values) {
    Type_ *ptr = Allocate<Type_>(1);
    new (ptr) Type_(values...);
    return ptr;
}

template <typename Type_>
inline static Type_ *AllocateInit() {
    Type_ *ptr = Allocate<Type_>(1);
    new (ptr) Type_();
    return ptr;
}

template <typename Type_>
inline static void Construct(Type_ *ptr, const Type_ &value) noexcept {
    new (ptr) Type_{value};
}

template <typename Type_>
inline static void Construct(Type_ *ptr, Type_ &&value) noexcept {
    new (ptr) Type_{static_cast<Type_ &&>(value)};
}

template <typename Type_>
inline static void Construct(Type_ *start, const Type_ *end,
                             const Type_ &value) noexcept {
    while (start < end) {
        new (start) Type_{value};
        ++start;
    }
}

template <typename Type_, typename... Values_T_>
inline static void ConstructValues(Type_ *ptr, Values_T_ &&... values) {
    new (ptr) Type_{static_cast<Values_T_ &&>(values)...};
}

template <typename Type_, typename... Values_T_>
inline static void ConstructValues(Type_ *ptr, const Values_T_ &... values) {
    new (ptr) Type_{values...};
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

} // namespace Memory
} // namespace Qentem

#endif
