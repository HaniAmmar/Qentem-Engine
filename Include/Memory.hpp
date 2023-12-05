/*
 * Copyright (c) 2023 Hani Ammar
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

#include "Platform.hpp"

#ifndef QENTEM_MEMORY_H_
#define QENTEM_MEMORY_H_

namespace Qentem::Memory {

// size = the number of bytes
template <SizeT32 Width_T_, typename Number_T_>
inline static void SetToZero(void *pointer, Number_T_ size) noexcept {
    Number_T_ offset = 0;

#ifdef QENTEM_SIMD_ENABLED
    const Number_T_ m_size = size;
    m_size >>= Platform::SIMD::Shift;

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
#endif

    if constexpr (Width_T_ >= SizeT32{8}) {
        SizeT64 *src = (SizeT64 *)(pointer);

        size >>= 3U;

        while (offset < size) {
            src[offset] = 0;
            ++offset;
        }
    } else {
        char *src = (char *)(pointer);

        while (offset < size) {
            src[offset] = 0;
            ++offset;
        }
    }
}

// size = the number of bytes
template <SizeT32 Width_T_, typename Number_T_>
QENTEM_NOINLINE static void Copy(void *to, const void *from, Number_T_ size) noexcept {
    Number_T_ offset = 0;

#ifdef QENTEM_SIMD_ENABLED
    const Number_T_ m_size = size;
    m_size >>= Platform::SIMD::Shift;

    if (m_size != 0) {
        offset = m_size;
        offset <<= Platform::SIMD::Shift;

        Platform::SIMD::VAR_T       *m_to   = (Platform::SIMD::VAR_T *)(to);
        const Platform::SIMD::VAR_T *m_form = (const Platform::SIMD::VAR_T *)(from);
        const Platform::SIMD::VAR_T *end    = (m_form + m_size);

        do {
            Platform::SIMD::Store(m_to, Platform::SIMD::Load(m_form));
            ++m_form;
            ++m_to;
        } while (m_form < end);
    }
#endif

    if constexpr (Width_T_ >= SizeT32{8}) {
        SizeT64       *des = (SizeT64 *)(to);
        const SizeT64 *src = (const SizeT64 *)(from);

        size >>= 3U;

        while (offset < size) {
            des[offset] = src[offset];
            ++offset;
        }
    } else {
        char       *des = (char *)(to);
        const char *src = (const char *)(from);

        while (offset < size) {
            des[offset] = src[offset];
            ++offset;
        }
    }
}
/////////////////////////////////////////////////////////////////////
template <typename Type_T_>
static constexpr Type_T_ *ChangePointer(void *value) noexcept {
    return (Type_T_ *)(value);
}

template <typename Type_T_>
static constexpr const Type_T_ *ChangePointer(const void *value) noexcept {
    return (const Type_T_ *)(value);
}
/////////////////////////////////////////////////////////////////////
template <typename Type_T_>
static constexpr Type_T_ &&Move(Type_T_ &value) noexcept {
    return (Type_T_ &&)(value);
}

template <typename Type_T_>
static constexpr Type_T_ &&Move(Type_T_ &&value) noexcept {
    return (Type_T_ &&)(value);
}
/////////////////////////////////////////////////////////////////////
template <typename Type_T_>
inline static void Swap(Type_T_ &item1, Type_T_ &item2) noexcept {
    Type_T_ item = Move(item1);
    item1        = Move(item2);
    item2        = Move(item);
}

template <bool Ascend_T_, typename Type_T_, typename Number_T_>
inline static void Sort(Type_T_ *arr, Number_T_ start, Number_T_ end) noexcept {
    if (start != end) {
        Type_T_  &item   = arr[start];
        Number_T_ index  = start;
        Number_T_ offset = (start + Number_T_{1});

        while (offset < end) {
            if constexpr (Ascend_T_) {
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

        Sort<Ascend_T_>(arr, start, index);
        ++index;
        Sort<Ascend_T_>(arr, index, end);
    }
}

/////////////////////////////////////////////////////////////////////
inline static SizeT AlignSize(SizeT n_size) noexcept {
    SizeT size = SizeT(SizeT{1} << Platform::FindLastBit(n_size));

    if (size < n_size) {
        size <<= 1U;
    }

    return size;
}
/////////////////////////////////////////////////////////////////////
template <typename Type_T_>
inline static Type_T_ *Allocate(SizeT size) {
    Type_T_ *pointer = ChangePointer<Type_T_>(::operator new(SystemIntType(size * sizeof(Type_T_))));

#ifdef QENTEM_TEST_HELPER_H_
    MemoryRecord::AddAllocation(pointer);
#endif
    // TODO: Build Allocator
    return pointer;
}

// Initializer
template <typename Type_T_>
inline static void Initialize(Type_T_ *pointer) noexcept {
    new (pointer) Type_T_{};
}

// Range copy initializer
template <typename Type_T_>
inline static void Initialize(Type_T_ *pointer, const Type_T_ *end) noexcept {
    while (pointer < end) {
        new (pointer) Type_T_{};
        ++pointer;
    }
}

// Move initializer
template <typename Type_T_>
inline static void Initialize(Type_T_ *pointer, Type_T_ &&value) noexcept {
    new (pointer) Type_T_{Move(value)};
}

// Copy initializer
template <typename Type_T_>
inline static void Initialize(Type_T_ *pointer, const Type_T_ &value) {
    new (pointer) Type_T_{value};
}

// Range copy initializer
template <typename Type_T_>
inline static void Initialize(Type_T_ *pointer, const Type_T_ *end, const Type_T_ &value) {
    while (pointer < end) {
        new (pointer) Type_T_{value};
        ++pointer;
    }
}

template <typename Type_T_, typename... Values_T_>
inline static void InitializeValues(Type_T_ *pointer, Values_T_ &&...values) noexcept {
    new (pointer) Type_T_{Move(values)...};
}

template <typename Type_T_, typename... Values_T_>
inline static void InitializeValues(Type_T_ *pointer, const Values_T_ &...values) noexcept {
    new (pointer) Type_T_{values...};
}

template <typename Type_T_>
inline static Type_T_ *AllocateInit() {
    Type_T_ *pointer = Allocate<Type_T_>(1);
    Initialize(pointer);
    return pointer;
    // return new Type_T_{};
}

// Allocate and move
template <typename Type_T_, typename... Values_T_>
inline static Type_T_ *AllocateInit(Values_T_ &&...values) noexcept {
    Type_T_ *pointer = Allocate<Type_T_>(1);
    InitializeValues(pointer, Move(values)...);
    return pointer;
}

// Allocate and copy
template <typename Type_T_, typename... Values_T_>
inline static Type_T_ *AllocateInit(const Values_T_ &...values) {
    Type_T_ *pointer = Allocate<Type_T_>(1);
    InitializeValues(pointer, values...);
    return pointer;
    // return new Type_T_{values...};
}

inline static void Deallocate(void *pointer) noexcept {
#ifdef QENTEM_TEST_HELPER_H_
    if (pointer != nullptr) {
        MemoryRecord::RemoveAllocation(pointer);
    }
#endif
    ::operator delete(pointer);
}

template <typename Type_T_>
inline static void Dispose(Type_T_ *item) noexcept {
    if (item != nullptr) {
        item->~Type_T_();
    }
}

template <typename Type_T_>
inline static void Dispose(Type_T_ *item, const Type_T_ *end) noexcept {
    while (item < end) {
        Dispose(item);
        ++item;
    }
}

} // namespace Qentem::Memory

#endif
