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

#ifndef _QENTEM_MEMORY_H
#define _QENTEM_MEMORY_H

namespace Qentem {
namespace Memory {

// size = the number of bytes
template <SizeT32 _Width_T, typename _Number_T>
inline static void SetToZero(void *pointer, _Number_T size) noexcept {
    _Number_T offset = 0;

    if (Config::IsSIMDEnabled) {
        const _Number_T m_size = (size >> Platform::SIMD::Shift);

        if (m_size != _Number_T{0}) {
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
template <SizeT32 _Width_T, typename _Number_T>
QENTEM_NOINLINE static void Copy(void *to, const void *from, _Number_T size) noexcept {
    _Number_T offset = 0;

    if (Config::IsSIMDEnabled) {
        const _Number_T m_size = (size >> Platform::SIMD::Shift);

        if (m_size != _Number_T{0}) {
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
    }

    char       *des = (char *)(to);
    const char *src = (const char *)(from);

    while (offset < size) {
        des[offset] = src[offset];
        ++offset;
    }
}
/////////////////////////////////////////////////////////////////////
template <typename _Type_T>
static constexpr _Type_T *ChangePointer(void *value) noexcept {
    return (_Type_T *)(value);
}

template <typename _Type_T>
static constexpr const _Type_T *ChangePointer(const void *value) noexcept {
    return (const _Type_T *)(value);
}
/////////////////////////////////////////////////////////////////////
template <typename _Type_T>
static constexpr _Type_T &&Move(_Type_T &value) noexcept {
    return (_Type_T &&)(value);
}

template <typename _Type_T>
static constexpr _Type_T &&Move(_Type_T &&value) noexcept {
    return (_Type_T &&)(value);
}
/////////////////////////////////////////////////////////////////////
template <typename _Type_T>
inline static void Swap(_Type_T &item1, _Type_T &item2) noexcept {
    _Type_T item = Move(item1);
    item1        = Move(item2);
    item2        = Move(item);
}

template <bool _Ascend_T, typename _Type_T, typename _Number_T>
inline static void Sort(_Type_T *arr, _Number_T start, _Number_T end) noexcept {
    if (start != end) {
        _Type_T  &item   = arr[start];
        _Number_T index  = start;
        _Number_T offset = (start + _Number_T{1});

        while (offset < end) {
            if (_Ascend_T) {
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

        Sort<_Ascend_T>(arr, start, index);
        ++index;
        Sort<_Ascend_T>(arr, index, end);
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
template <typename _Type_T>
inline static _Type_T *Allocate(SizeT size) {
    const SystemIntType byte_size = SystemIntType(size * sizeof(_Type_T));
    _Type_T            *pointer   = ChangePointer<_Type_T>(::operator new(byte_size));

#ifdef _QENTEM_Q_TEST_H
    MemoryRecord::AddAllocation(pointer);
#endif
    // TODO: Build Allocator
    return pointer;
}

// Initializer
template <typename _Type_T>
inline static void Initialize(_Type_T *pointer) noexcept {
    new (pointer) _Type_T{};
}

// Range copy initializer
template <typename _Type_T>
inline static void Initialize(_Type_T *pointer, const _Type_T *end) noexcept {
    while (pointer < end) {
        new (pointer) _Type_T{};
        ++pointer;
    }
}

// Move initializer
template <typename _Type_T>
inline static void Initialize(_Type_T *pointer, _Type_T &&value) noexcept {
    new (pointer) _Type_T{Move(value)};
}

// Copy initializer
template <typename _Type_T>
inline static void Initialize(_Type_T *pointer, const _Type_T &value) {
    new (pointer) _Type_T{value};
}

// Range copy initializer
template <typename _Type_T>
inline static void Initialize(_Type_T *pointer, const _Type_T *end, const _Type_T &value) {
    while (pointer < end) {
        new (pointer) _Type_T{value};
        ++pointer;
    }
}

template <typename _Type_T, typename... _Values_T>
inline static void InitializeValues(_Type_T *pointer, _Values_T &&...values) noexcept {
    new (pointer) _Type_T{Move(values)...};
}

template <typename _Type_T, typename... _Values_T>
inline static void InitializeValues(_Type_T *pointer, const _Values_T &...values) noexcept {
    new (pointer) _Type_T{values...};
}

template <typename _Type_T>
inline static _Type_T *AllocateInit() {
    _Type_T *pointer = Allocate<_Type_T>(1);
    Initialize(pointer);
    return pointer;
    // return new _Type_T{};
}

// Allocate and move
template <typename _Type_T, typename... _Values_T>
inline static _Type_T *AllocateInit(_Values_T &&...values) noexcept {
    _Type_T *pointer = Allocate<_Type_T>(1);
    InitializeValues(pointer, Move(values)...);
    return pointer;
}

// Allocate and copy
template <typename _Type_T, typename... _Values_T>
inline static _Type_T *AllocateInit(const _Values_T &...values) {
    _Type_T *pointer = Allocate<_Type_T>(1);
    InitializeValues(pointer, values...);
    return pointer;
    // return new _Type_T{values...};
}

inline static void Deallocate(void *pointer) noexcept {
#ifdef _QENTEM_Q_TEST_H
    if (pointer != nullptr) {
        MemoryRecord::RemoveAllocation(pointer);
    }
#endif
    ::operator delete(pointer);
}

template <typename _Type_T>
inline static void Dispose(_Type_T *item) noexcept {
    if (item != nullptr) {
        item->~_Type_T();
    }
}

template <typename _Type_T>
inline static void Dispose(_Type_T *item, const _Type_T *end) noexcept {
    while (item < end) {
        Dispose(item);
        ++item;
    }
}

} // namespace Memory
} // namespace Qentem

#endif
