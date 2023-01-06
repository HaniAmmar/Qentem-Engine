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
    QPointer(QPointer &&src) noexcept : pointer_{src.pointer_} { src.pointer_ = nullptr; }

    QPointer &operator=(QPointer &&src) noexcept {
        if (this != &src) {
            pointer_     = src.pointer_;
            src.pointer_ = nullptr;
        }

        return *this;
    }

    void SetPointer(Type_T_ *pointer) noexcept {
        if (Config::PointerTagging) {
            p_number_ &= 0xFFFF000000000000ULL;
            p_number_ |= (reinterpret_cast<unsigned long long>(pointer) & 0x0000FFFFFFFFFFFFULL);
        } else {
            pointer_ = pointer;
        }
    }

    Type_T_ *GetPointer() const noexcept {
        if (Config::PointerTagging) {
            return reinterpret_cast<Type_T_ *>((p_number_ & 0x0000FFFFFFFFFFFFULL));
        } else {
            return pointer_;
        }
    }

    void MovePointerOnly(QPointer &src) noexcept {
        if (Config::PointerTagging) {
            bits_.number_ = src.bits_.number_;
        } else {
            pointer_ = src.pointer_;
        }

        src.pointer_ = nullptr;
    }

    void          SetHighByte(unsigned char byte) noexcept { bits_.high_byte_ = byte; }
    unsigned char GetHighByte() const noexcept { return bits_.high_byte_; }
    void          SetLowByte(unsigned char byte) noexcept { bits_.low_byte_ = byte; }
    unsigned char GetLowByte() const noexcept { return bits_.low_byte_; }

    void Reset() noexcept { pointer_ = nullptr; }

  private:
    struct Bits {
#ifndef QENTEM_BIG_ENDIAN
        unsigned long long number_ : 48;
        unsigned long long low_byte_ : 8;
        unsigned long long high_byte_ : 8;
#else
        unsigned long long high_byte_ : 8;
        unsigned long long low_byte_ : 8;
        unsigned long long number_ : 48;
#endif
    };

    union {
        Type_T_           *pointer_{nullptr};
        unsigned long long p_number_;
        Bits               bits_;
    };
};

} // namespace Qentem

#endif
