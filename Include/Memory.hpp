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
template <typename Number_T_>
inline static void SetToZero(void *pointer, Number_T_ size) noexcept {
    Number_T_ offset = 0;

#ifdef QENTEM_SIMD_ENABLED
    const Number_T_ m_size = (size >> Platform::SIMD::ShiftWidth);

    if (m_size != 0) {
        offset += (Platform::SIMD::Size * m_size);
        const Platform::SIMD::VAR_T  m_zero    = Platform::SIMD::Zero();
        Platform::SIMD::VAR_T       *m_pointer = (Platform::SIMD::VAR_T *)(pointer);
        const Platform::SIMD::VAR_T *end       = (m_pointer + m_size);

        do {
            Platform::SIMD::Store(m_pointer, m_zero);
            ++m_pointer;
        } while (m_pointer < end);
    }
#endif

    char *src = (char *)(pointer);

    while (offset < size) {
        src[offset] = 0;
        ++offset;
    }
}

// size = the number of bytes
template <typename Number_T_>
inline static void Copy(void *to, const void *from, Number_T_ size) noexcept {
    Number_T_ offset = 0;

#ifdef QENTEM_SIMD_ENABLED
    const Number_T_ m_size = (size >> Platform::SIMD::ShiftWidth);

    if (m_size != 0) {
        offset += (Platform::SIMD::Size * m_size);
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

    char       *des = (char *)(to);
    const char *src = (const char *)(from);

    while (offset < size) {
        des[offset] = src[offset];
        ++offset;
    }
}
/////////////////////////////////////////////////////////////////////
template <typename Type_>
static constexpr Type_ *ChangePointer(void *value) noexcept {
    return (Type_ *)(value);
}

template <typename Type_>
static constexpr const Type_ *ChangePointer(const void *value) noexcept {
    return (const Type_ *)(value);
}
/////////////////////////////////////////////////////////////////////
template <typename Type_>
static constexpr Type_ &&Move(Type_ &value) noexcept {
    return (Type_ &&)(value);
}

template <typename Type_>
static constexpr Type_ &&Move(Type_ &&value) noexcept {
    return (Type_ &&)(value);
}
/////////////////////////////////////////////////////////////////////
template <typename Type_>
inline static void Swap(Type_ &item1, Type_ &item2) noexcept {
    Type_ item = Move(item1);
    item1      = Move(item2);
    item2      = Move(item);
}

template <typename Type_, typename Number_T_>
struct QuickSort {
  private:
    inline static void ascendSort(Type_ *arr, Number_T_ start, Number_T_ end) noexcept {
        if (start < end) {
            Type_    *item  = (arr + start);
            Number_T_ index = start;

            for (Number_T_ x = (start + 1); x < end; x++) {
                if (!((*(arr + x)) > *item)) {
                    ++index;
                    Swap((*(arr + index)), ((*(arr + x))));
                }
            }

            Swap((*(arr + index)), (*(arr + start)));

            ascendSort(arr, start, index);
            ascendSort(arr, (index + 1), end);
        }
    }

    inline static void descendSort(Type_ *arr, Number_T_ start, Number_T_ end) noexcept {
        if (start < end) {
            Type_    *item  = (arr + start);
            Number_T_ index = start;

            for (Number_T_ x = (start + 1); x < end; x++) {
                if (!((*(arr + x)) < *item)) {
                    ++index;
                    Swap((*(arr + index)), ((*(arr + x))));
                }
            }

            Swap((*(arr + index)), (*(arr + start)));

            descendSort(arr, start, index);
            descendSort(arr, (index + 1), end);
        }
    }

  public:
    inline static void Sort(Type_ *arr, Number_T_ start, Number_T_ end, bool ascend) noexcept {
        if (ascend) {
            ascendSort(arr, start, end);
        } else {
            descendSort(arr, start, end);
        }
    }

    QuickSort()                             = delete;
    QuickSort(QuickSort &&)                 = delete;
    QuickSort(const QuickSort &)            = delete;
    QuickSort &operator=(QuickSort &&)      = delete;
    QuickSort &operator=(const QuickSort &) = delete;
    ~QuickSort()                            = delete;
};

/////////////////////////////////////////////////////////////////////
inline static SizeT AlignSize(SizeT n_size) noexcept {
    const SizeT size = SizeT(SizeT{1} << Platform::FindLastBit(n_size));

    if (size < n_size) {
        return (size * SizeT{2});
    }

    return size;
}
/////////////////////////////////////////////////////////////////////
template <typename Type_>
inline static Type_ *Allocate(SizeT size) {
    Type_ *pointer = ChangePointer<Type_>(::operator new(SystemIntType(size * sizeof(Type_))));

#ifdef QENTEM_TEST_HELPER_H_
    MemoryRecord::AddAllocation(pointer);
#endif
    // TODO: Build Allocator
    return pointer;
}

// Initializer
template <typename Type_>
inline static void Initialize(Type_ *pointer) noexcept {
    new (pointer) Type_{};
}

// Range copy initializer
template <typename Type_>
inline static void Initialize(Type_ *pointer, const Type_ *end) noexcept {
    while (pointer < end) {
        new (pointer) Type_{};
        ++pointer;
    }
}

// Move initializer
template <typename Type_>
inline static void Initialize(Type_ *pointer, Type_ &&value) noexcept {
    new (pointer) Type_{Move(value)};
}

// Copy initializer
template <typename Type_>
inline static void Initialize(Type_ *pointer, const Type_ &value) {
    new (pointer) Type_{value};
}

// Range copy initializer
template <typename Type_>
inline static void Initialize(Type_ *pointer, const Type_ *end, const Type_ &value) {
    while (pointer < end) {
        new (pointer) Type_{value};
        ++pointer;
    }
}

template <typename Type_, typename... Values_T_>
inline static void InitializeValues(Type_ *pointer, Values_T_ &&...values) noexcept {
    new (pointer) Type_{Move(values)...};
}

template <typename Type_, typename... Values_T_>
inline static void InitializeValues(Type_ *pointer, const Values_T_ &...values) noexcept {
    new (pointer) Type_{values...};
}

template <typename Type_>
inline static Type_ *AllocateInit() {
    Type_ *pointer = Allocate<Type_>(1);
    Initialize(pointer);
    return pointer;
    // return new Type_{};
}

// Allocate and move
template <typename Type_, typename... Values_T_>
inline static Type_ *AllocateInit(Values_T_ &&...values) noexcept {
    Type_ *pointer = Allocate<Type_>(1);
    InitializeValues(pointer, Move(values)...);
    return pointer;
}

// Allocate and copy
template <typename Type_, typename... Values_T_>
inline static Type_ *AllocateInit(const Values_T_ &...values) {
    Type_ *pointer = Allocate<Type_>(1);
    InitializeValues(pointer, values...);
    return pointer;
    // return new Type_{values...};
}

inline static void Deallocate(void *pointer) noexcept {
#ifdef QENTEM_TEST_HELPER_H_
    if (pointer != nullptr) {
        MemoryRecord::RemoveAllocation(pointer);
    }
#endif
    ::operator delete(pointer);
}

template <typename Type_>
inline static void Dispose(Type_ *item, const Type_ *end) noexcept {
    while (item < end) {
        if (item != nullptr) {
            item->~Type_();
        }

        ++item;
    }
}

template <typename Type_>
inline static void Dispose(Type_ *item) noexcept {
    if (item != nullptr) {
        item->~Type_();
    }
}

} // namespace Qentem::Memory

#endif
