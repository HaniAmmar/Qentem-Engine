/*
 * Copyright (c) 2024 Hani Ammar
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

#ifndef QENTEM_MEMORY_H
#define QENTEM_MEMORY_H

#include "Platform.hpp"

namespace Qentem {
namespace Memory {
// size = the number of bytes
template <typename Number_T>
inline static void SetToZero(void *pointer, Number_T size) noexcept {
    Number_T offset = 0;

    if (Config::IsSIMDEnabled) {
        const Number_T m_size = (size >> Platform::SIMD::Shift);

        if (m_size != 0) {
            offset = m_size;
            offset <<= Platform::SIMD::Shift;

            const Platform::SIMD::VAR_T  m_zero    = Platform::SIMD::Zero();
            Platform::SIMD::VAR_T       *m_pointer = (Platform::SIMD::VAR_T *)(pointer);
            const Platform::SIMD::VAR_T *end       = (m_pointer + m_size);

            do {
                Platform::SIMD::Store(m_pointer, m_zero);
                ++m_pointer;
            } while (m_pointer < end);
        }
    }

    char *src = (char *)(pointer);

    while (offset < size) {
        src[offset] = 0;
        ++offset;
    }
}

// size = the number of bytes
template <typename Number_T>
QENTEM_NOINLINE static void Copy(void *to, const void *from, Number_T size) noexcept {
    Number_T offset = 0;

    // if (Config::IsSIMDEnabled) {
    //     const Number_T m_size = (size >> Platform::SIMD::Shift);

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
static constexpr Type_T *ChangePointer(void *value) noexcept {
    return (Type_T *)(value);
}

template <typename Type_T>
static constexpr const Type_T *ChangePointer(const void *value) noexcept {
    return (const Type_T *)(value);
}
/////////////////////////////////////////////////////////////////////
template <typename Type_T>
static constexpr Type_T &&Move(Type_T &value) noexcept {
    return (Type_T &&)(value);
}
/////////////////////////////////////////////////////////////////////
template <typename Type_T>
inline static void Swap(Type_T &item1, Type_T &item2) noexcept {
    Type_T item = Move(item1);
    item1       = Move(item2);
    item2       = Move(item);
}

template <bool Ascend_T, typename Type_T, typename Number_T>
inline static void Sort(Type_T *arr, Number_T start, Number_T end) noexcept {
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

/////////////////////////////////////////////////////////////////////
inline static SizeT AlignSize(SizeT n_size) noexcept {
    // Ensure scanned n_size is >= 2, so msb >= 1.
    SizeT size = SizeT(SizeT{1} << Platform::FindLastBit(n_size | 2));

    // If n_size wasn’t already exactly that power-of-two, bump up:
    if (size < n_size) {
        size <<= 1U;
    }

    return size;
}
/////////////////////////////////////////////////////////////////////
template <typename Type_T>
inline static Type_T *Allocate(SizeT size) {
    Type_T *pointer = ChangePointer<Type_T>(::operator new(SystemIntType(size * sizeof(Type_T))));

#ifdef QENTEM_Q_TEST_H
    MemoryRecord::AddAllocation(pointer);
#endif
    // TODO: Build Allocator
    return pointer;
}

// Initializer
template <typename Type_T>
inline static void Initialize(Type_T *pointer) noexcept {
    new (pointer) Type_T{};
}

// Range copy initializer
template <typename Type_T>
inline static void Initialize(Type_T *pointer, const Type_T *end) noexcept {
    while (pointer < end) {
        new (pointer) Type_T{};
        ++pointer;
    }
}

// Move initializer
template <typename Type_T>
inline static void Initialize(Type_T *pointer, Type_T &&value) noexcept {
    new (pointer) Type_T{Move(value)};
}

// Copy initializer
template <typename Type_T>
inline static void Initialize(Type_T *pointer, const Type_T &value) {
    new (pointer) Type_T{value};
}

// Range copy initializer
template <typename Type_T>
inline static void Initialize(Type_T *pointer, const Type_T *end, const Type_T &value) {
    while (pointer < end) {
        new (pointer) Type_T{value};
        ++pointer;
    }
}

template <typename Type_T, typename... Values_T>
inline static void InitializeValues(Type_T *pointer, Values_T &&...values) noexcept {
    new (pointer) Type_T{Move(values)...};
}

template <typename Type_T, typename... Values_T>
inline static void InitializeValues(Type_T *pointer, const Values_T &...values) noexcept {
    new (pointer) Type_T{values...};
}

template <typename Type_T>
inline static Type_T *AllocateInit() {
    Type_T *pointer = Allocate<Type_T>(1);
    Initialize(pointer);
    return pointer;
    // return new Type_T{};
}

// Allocate and move
template <typename Type_T, typename... Values_T>
inline static Type_T *AllocateInit(Values_T &&...values) noexcept {
    Type_T *pointer = Allocate<Type_T>(1);
    InitializeValues(pointer, Move(values)...);
    return pointer;
}

// Allocate and copy
template <typename Type_T, typename... Values_T>
inline static Type_T *AllocateInit(const Values_T &...values) {
    Type_T *pointer = Allocate<Type_T>(1);
    InitializeValues(pointer, values...);
    return pointer;
    // return new Type_T{values...};
}

inline static void Deallocate(void *pointer) noexcept {
#ifdef QENTEM_Q_TEST_H
    if (pointer != nullptr) {
        MemoryRecord::RemoveAllocation(pointer);
    }
#endif
    ::operator delete(pointer);
}

template <typename Type_T>
inline static void Dispose(Type_T *item) noexcept {
    if (item != nullptr) {
        item->~Type_T();
    }
}

template <typename Type_T>
inline static void Dispose(Type_T *item, const Type_T *end) noexcept {
    while (item < end) {
        Dispose(item);
        ++item;
    }
}

} // namespace Memory
} // namespace Qentem

#endif
