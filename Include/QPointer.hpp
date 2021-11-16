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
    // Only 64-bit uses pointer tagging, so there is no need to adjust its size.
  private:
    struct Tag_ {
#ifndef QENTEM_BIG_ENDIAN
        unsigned long long int_ptr_ : 48;
        unsigned long long low_ : 8;
        unsigned long long high_ : 8;
#else
        unsigned long long high_ : 8;
        unsigned long long low_ : 8;
        unsigned long long int_ptr_ : 48;
#endif
    };

  public:
    QPointer() noexcept : ptr_{nullptr} {}
    ~QPointer() = default;
    explicit QPointer(Type_ *ptr) noexcept : ptr_{ptr} {}
    QPointer(QPointer &&src) noexcept : ptr_{src.ptr_} { src.ptr_ = nullptr; }
    QPointer(const QPointer &src) noexcept : ptr_{src.ptr_} {}
    QPointer &operator=(QPointer &&src) noexcept {
        if (this != &src) {
            SetPointer(src.ptr_);
            src.ptr_ = nullptr;
        }

        return *this;
    }

    QPointer &operator=(const QPointer &src) noexcept {
        if (this != &src) {
            SetPointer(src.ptr_);
        }

        return *this;
    }

    void Reset() noexcept { ptr_ = nullptr; }

    void Set(QPointer &&src) noexcept {
        ptr_     = src.ptr_;
        src.ptr_ = nullptr;
    }

    void Set(const QPointer &src) noexcept { ptr_ = src.ptr_; }

    void SetPointer(Type_ *ptr) noexcept {
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
        const unsigned char tag_h = tag_.high_;
        const unsigned char tag_l = tag_.low_;

        ptr_       = ptr;
        tag_.high_ = tag_h;
        tag_.low_  = tag_l;
#else
        ptr_ = ptr;
#endif
    }

    Type_ *GetPointer() const noexcept {
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
        return reinterpret_cast<Type_ *>(tag_.int_ptr_);
#else
        return ptr_;
#endif
    }

#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
    void          SetHighTag(unsigned char tag) noexcept { tag_.high_ = tag; }
    unsigned char GetHighTag() const noexcept { return tag_.high_; }

    void          SetLowTag(unsigned char tag) noexcept { tag_.low_ = tag; }
    unsigned char GetLowTag() const noexcept { return tag_.low_; }
#endif

    union {
        Type_ *ptr_;
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
        Tag_ tag_;
#endif
    };
};

} // namespace Qentem

#endif
