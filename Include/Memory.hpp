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

inline static void SetToZero(void *ptr, ULong size) {
#ifdef QENTEM_SIMD_ENABLED_
    const ULong m_size    = (size >> QMM_SHIFTSIZE_);
    const ULong remaining = (size ^ (m_size << QMM_SHIFTSIZE_));

    if (m_size != 0) {
        QMM_VAR_ *      m_ptr  = static_cast<QMM_VAR_ *>(ptr);
        const QMM_VAR_ *end    = (m_ptr + m_size);
        const QMM_VAR_  m_zero = QMM_SETZERO_();

        do {
            QMM_STOREU_(m_ptr, m_zero);
            ++m_ptr;
        } while (m_ptr != end);
    }

    if (remaining != 0) {
        char *      des = (static_cast<char *>(ptr) + (size - remaining));
        const char *end = (des + remaining);

        do {
            *des = 0;
            ++des;
        } while (des != end);
    }
#else
    char *      des = static_cast<char *>(ptr);
    const char *end = (des + size);

    while (des != end) {
        *des = 0;
        ++des;
    }
#endif
}

static void Copy(void *to, const void *form, ULong size) {
#ifdef QENTEM_SIMD_ENABLED_
    const ULong m_size    = (size >> QMM_SHIFTSIZE_);
    const ULong remaining = (size ^ (m_size << QMM_SHIFTSIZE_));

    if (m_size != 0) {
        QMM_VAR_ *      m_to   = static_cast<QMM_VAR_ *>(to);
        const QMM_VAR_ *m_form = static_cast<const QMM_VAR_ *>(form);
        const QMM_VAR_ *end    = (m_form + m_size);

        do {
            QMM_STOREU_(m_to, QMM_LOAD_(m_form));
            ++m_form;
            ++m_to;
        } while (m_form != end);
    }

    if (remaining != 0) {
        const ULong start = (size - remaining);
        const char *src   = static_cast<const char *>(form) + start;
        const char *end   = (src + remaining);

        char *des = static_cast<char *>(to) + start;

        do {
            *des = *src;
            ++des;
            ++src;
        } while (src != end);
    }
#else
    const char *src = static_cast<const char *>(form);
    const char *end = (src + size);
    char *      des = static_cast<char *>(to);

    while (src != end) {
        *des = *src;
        ++des;
        ++src;
    }
#endif
}

inline static bool Compare(const char *left_text, const char *right_text,
                           ULong length) noexcept {
    ULong offset = 0;
#ifdef QENTEM_SIMD_ENABLED_
    if ((left_text != nullptr) && (right_text != nullptr)) {
        while (offset < length) {
            const QMM_VAR_ mm_l = QMM_LOAD_(
                reinterpret_cast<const QMM_VAR_ *>(left_text + offset));

            const QMM_VAR_ mm_r = QMM_LOAD_(
                reinterpret_cast<const QMM_VAR_ *>(right_text + offset));

            const unsigned long long bits = QMM_COMPARE_8_MASK_(mm_l, mm_r);

            // if (bits == 0) {
            //     break; // No match
            // }

            if ((Q_CTZL(bits + 1) + offset) >= length) {
                return true;
            }

            offset += QMM_SIZE_;

            if ((offset >= length) || (bits != QMM_MAX_NUMBER)) {
                return false;
            }
        }
    }
#else
    while (offset != length) {
        if (left_text[offset] != right_text[offset]) {
            break;
        }

        ++offset;
    }
#endif

    return (length == offset);
}

} // namespace Memory

class HAllocator {
  public:
    template <typename Type>
    inline static Type *Allocate(ULong size) {
        return static_cast<Type *>(malloc(size * sizeof(Type)));
    }

    template <typename Type>
    inline static Type *Allocate(Type &&value) {
        Type *ptr = Allocate<Type>(1);
        new (ptr) Type(static_cast<Type &&>(value));
        return ptr;
    }

    // template <typename Type>
    // static Type *Allocate(ULong size, Type &&value) {
    //     Type *ptr = Allocate<Type>(size);

    //     ULong i = 0;
    //     while (i != size) {
    //         *(ptr + i) = value;
    //         ++i;
    //     }

    //     return ptr;
    // }

    template <typename Type>
    inline static Type *AllocateClear(ULong size) {
        const ULong c_size = (size * sizeof(Type));
        void *      vptr   = malloc(c_size);
        Memory::SetToZero(vptr, c_size);

        return static_cast<Type *>(vptr);
    }

    // template <typename Type>
    // inline static Type *AllocateInit() {
    //     Type *ptr = Allocate<Type>(1);
    //     new (ptr) Type();
    //     return ptr;
    // }

    // template <typename Type>
    // static Type *AllocateInit(ULong size) {
    //     Type *ptr = Allocate<Type>(size);

    //     ULong i = 0;
    //     while (i != size) {
    //         new (ptr + i) Type();
    //         ++i;
    //     }

    //     return ptr;
    // }

    template <typename Type>
    inline static void Construct(Type *ptr, Type &&value) {
        new (ptr) Type(static_cast<Type &&>(value));
    }

    template <typename Type>
    inline static void Construct(Type *start, const Type *end, Type &&value) {
        while (start != end) {
            new (start) Type(static_cast<Type &&>(value));
            ++start;
        }
    }

    template <typename Type>
    inline static void Destruct(Type *ptr) {
        ptr->~Type();
    }

    template <typename Type>
    inline static void Destruct(Type *start, const Type *end) {
        while (start != end) {
            start->~Type();
            ++start;
        }
    }

    inline static void Deallocate(void *ptr) {
        if (ptr != nullptr) {
            free(ptr);
        }
    }
};

} // namespace Qentem

#endif
