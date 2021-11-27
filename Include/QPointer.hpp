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

#include "Memory.hpp"

#ifndef QENTEM_Q_POINTER_H_
#define QENTEM_Q_POINTER_H_

namespace Qentem {

template <typename Type_>
class QPointer {
  public:
    QPointer() noexcept : ptr_{nullptr} {}
    ~QPointer()                   = default;
    QPointer(const QPointer &src) = delete;
    QPointer &operator=(const QPointer &src) = delete;

    explicit QPointer(Type_ *ptr) noexcept : ptr_{ptr} {}

    QPointer(QPointer &&src) noexcept : ptr_{src.ptr_} { src.ptr_ = nullptr; }

    void operator=(QPointer &&src) noexcept {
        SetPointer(src.ptr_);
        src.ptr_ = nullptr;
    }

    void SetPointer(Type_ *ptr) noexcept {
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
        bits_.num_ = (QPointer(ptr)).bits_.num_;
#else
        ptr_ = ptr;
#endif
    }

    Type_ *GetPointer() const noexcept {
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
        return reinterpret_cast<Type_ *>(bits_.num_);
#else
        return ptr_;
#endif
    }

    void Move(QPointer &&src) noexcept {
        ptr_     = src.ptr_;
        src.ptr_ = nullptr;
    }

#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
    void          SetHighTag(unsigned char tag) noexcept { bits_.high_ = tag; }
    unsigned char GetHighTag() const noexcept { return bits_.high_; }

    void          SetLowTag(unsigned char tag) noexcept { bits_.low_ = tag; }
    unsigned char GetLowTag() const noexcept { return bits_.low_; }
#endif

    void Reset() noexcept { ptr_ = nullptr; }

    // Only 64-bit uses pointer tagging, so there is no need to adjust its size.
  private:
    union {
        Type_ *ptr_;
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
        struct {
#ifndef QENTEM_BIG_ENDIAN
            unsigned long long num_ : 48;
            unsigned long long low_ : 8;
            unsigned long long high_ : 8;
#else
            unsigned long long high_ : 8;
            unsigned long long low_ : 8;
            unsigned long long num_ : 48;
#endif
        } bits_;
#endif
    };
};

} // namespace Qentem

#endif
