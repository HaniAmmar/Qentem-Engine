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

#ifndef QENTEM_Q_POINTER_H_
#define QENTEM_Q_POINTER_H_

namespace Qentem {

template <typename Type_T_>
class QPointer {
  public:
    QPointer()                               = default;
    ~QPointer()                              = default;
    QPointer(const QPointer &src)            = delete;
    QPointer &operator=(const QPointer &src) = delete;

    explicit QPointer(Type_T_ *pointer) noexcept : pointer_{pointer} {}
    QPointer(QPointer &&src) noexcept : p_number_{src.p_number_} { src.p_number_ = 0; }

    QPointer &operator=(QPointer &&src) noexcept {
        if (this != &src) {
            p_number_ &= 0xFFFF000000000000ULL;
            p_number_ |= (src.p_number_ & 0x0000FFFFFFFFFFFFULL);
            src.p_number_ = 0;
        }

        return *this;
    }

    void SetPointer(Type_T_ *pointer) noexcept {
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
        p_number_ &= 0xFFFF000000000000ULL;
        p_number_ |= (reinterpret_cast<unsigned long long>(pointer) & 0x0000FFFFFFFFFFFFULL);
#else
        pointer_ = pointer;
#endif
    }

    Type_T_ *GetPointer() const noexcept {
#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
        char *tmp_pointer = nullptr;
        tmp_pointer += (p_number_ & 0x0000FFFFFFFFFFFFULL);
        return reinterpret_cast<Type_T_ *>(tmp_pointer);
#else
        return pointer_;
#endif
    }

    void Move(QPointer &&src) noexcept {
        pointer_     = src.pointer_;
        src.pointer_ = nullptr;
    }

#if defined(QENTEM_POINTER_TAGGING) && (QENTEM_POINTER_TAGGING == 1)
    void SetHighByte(unsigned char byte) noexcept {
        unsigned long long tmp = byte;
        p_number_ &= 0x00FFFFFFFFFFFFFFULL;
        p_number_ |= (tmp << 56U);
    }

    unsigned char GetHighByte() const noexcept { return static_cast<unsigned char>(p_number_ >> 56U); }

    void SetLowByte(unsigned char byte) noexcept {
        unsigned long long tmp = byte;
        p_number_ &= 0xFF00FFFFFFFFFFFFULL;
        p_number_ |= (tmp << 48U);
    }

    unsigned char GetLowByte() const noexcept { return ((p_number_ >> 48U) & 0x00FFU); }
#endif

    void Reset() noexcept { pointer_ = nullptr; }

  private:
    union {
        Type_T_           *pointer_;
        unsigned long long p_number_{0};
    };
};

} // namespace Qentem

#endif
