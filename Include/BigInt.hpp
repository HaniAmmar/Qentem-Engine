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

#ifndef QENTEM_BIG_INT_H_
#define QENTEM_BIG_INT_H_

namespace Qentem {

// Width_T_ is the needed number of bits:
// 2 64bit int = 128 bits
// 6 64bit int = 256 bits
// 16 64bit int = 1024 bits
// (number of variables) * sizeof(variable) * 8 = Width_T_

template <typename Number_T_, unsigned int Width_T_>
struct BigInt {
    BigInt()  = default;
    ~BigInt() = default;

    BigInt(const Number_T_ number) noexcept {
        big_int_[0U] = number;
    }

    void operator=(const Number_T_ number) noexcept {
        big_int_[0U] = number;

        while (index_ != 0U) {
            big_int_[index_] = Number_T_{0};
            --index_;
        }
    }

    inline friend bool operator<(const BigInt &out, const Number_T_ number) noexcept {
        return (((out.index_ == 0U) && (out.big_int_[0U] < number)));
    }

    inline friend bool operator<(const Number_T_ number, const BigInt &out) noexcept {
        return (out > number);
    }

    inline friend bool operator<=(const BigInt &out, const Number_T_ number) noexcept {
        return (((out.index_ == 0U) && (out.big_int_[0U] <= number)));
    }

    inline friend bool operator<=(const Number_T_ number, const BigInt &out) noexcept {
        return (out >= number);
    }

    inline friend bool operator>(const BigInt &out, const Number_T_ number) noexcept {
        return (((out.index_ != 0U) || (out.big_int_[0U] > number)));
    }

    inline friend bool operator>(const Number_T_ number, const BigInt &out) noexcept {
        return (out < number);
    }

    inline friend bool operator>=(const BigInt &out, const Number_T_ number) noexcept {
        return (((out.index_ != 0U) || (out.big_int_[0U] >= number)));
    }

    inline friend bool operator>=(const Number_T_ number, const BigInt &out) noexcept {
        return (out <= number);
    }

    inline friend bool operator==(const BigInt &out, const Number_T_ number) noexcept {
        return (((out.index_ == 0U) && (out.big_int_[0U] == number)));
    }

    inline friend bool operator==(const Number_T_ number, const BigInt &out) noexcept {
        return (out == number);
    }

    inline friend bool operator!=(const BigInt &out, const Number_T_ number) noexcept {
        return (((out.index_ != 0U) || (out.big_int_[0U] != number)));
    }

    inline friend bool operator!=(const Number_T_ number, const BigInt &out) noexcept {
        return (out != number);
    }

    inline void operator<<=(const unsigned int number) noexcept {
        ShiftLeft(number);
    }

    inline void operator>>=(const unsigned int number) noexcept {
        ShiftRight(number);
    }

    inline void operator|=(const Number_T_ number) noexcept {
        big_int_[0U] |= number;
    }

    inline void operator&=(const Number_T_ number) noexcept {
        big_int_[0U] &= number;
    }

    inline void operator+=(const Number_T_ number) noexcept {
        Add(number);
    }

    inline void operator-=(const Number_T_ number) noexcept {
        Subtract(number);
    }
    ////////////////////////////////////////////////////
    void Add(const Number_T_ number, const unsigned int index = 0U) noexcept {
        if ((number != 0U) && (index <= MaxIndex)) {
            const Number_T_ tmp = big_int_[index];
            big_int_[index] += number;

            if (big_int_[index] < tmp) {
                // Overflow.
                Add(Number_T_{1}, (index + 1U));
            }

            index_ += (index > index_);
        }
    }

    inline void Subtract(const Number_T_ number, const unsigned int index = 0U) noexcept {
        if ((number != 0U) && (index <= MaxIndex)) {
            const Number_T_ tmp = big_int_[index];
            big_int_[index] -= number;

            if (big_int_[index] > tmp) {
                // Underflow.
                subtract(Number_T_{1}, (index + 1U));
            }

            index_ -= ((index_ > 0U) && (big_int_[index_] == 0U));
        }
    }
    ////////////////////////////////////////////////////
    template <typename Number_T2>
    inline static Number_T_ DoubleSizeMultiply(Number_T_ &number, const Number_T2 multiplier) noexcept {
        static constexpr unsigned int shift_size = (sizeof(Number_T_) * 8U);

        unsigned long long number64 = number;
        number64 *= multiplier;
        number = static_cast<Number_T_>(number64 & 0xFFFFFFFFU);

        return static_cast<Number_T_>((number64 >> shift_size) & 0xFFFFFFFFU);
    }

#ifdef QENTEM_64BIT_ARCH
    inline static unsigned long long DoubleSizeMultiply(unsigned long long      &number,
                                                        const unsigned long long multiplier) noexcept {
#ifdef _MSC_VER
        unsigned long long carry = 0;

        number = _umul128(multiplier, number, &carry);

        return carry;
#else
        static constexpr unsigned int shift_size = (sizeof(long long) * 8U);

        __uint128_t number128 = number;
        number128 *= multiplier;
        number = static_cast<unsigned long long>(number128 & 0xFFFFFFFFFFFFFFFFULL);

        return static_cast<unsigned long long>((number128 >> shift_size) & 0xFFFFFFFFFFFFFFFFULL);
#endif
    }
#endif

    inline void MultiplyBy(Number_T_ multiplier) noexcept {
        unsigned int index = (index_ + 1U);

        do {
            --index;

            if (big_int_[index] != 0U) {
                Add(DoubleSizeMultiply(big_int_[index], multiplier), (index + 1U));
            }
        } while (index != 0U);
    }
    ////////////////////////////////////////////////////
    template <typename Number_T2>
    static inline void DoubleSizeDivide(Number_T_ &dividend_high, Number_T_ &dividend_low,
                                        const Number_T2 divisor) noexcept {
        static constexpr unsigned int shift_size = (sizeof(Number_T_) * 8U);

        unsigned long long dividend64 = dividend_high;
        dividend64 <<= shift_size;
        dividend64 |= dividend_low;
        dividend_high = static_cast<Number_T_>(dividend64 % divisor);
        dividend64 /= divisor;
        dividend_low = static_cast<Number_T_>(dividend64 & 0xFFFFFFFFU);
    }

#ifdef QENTEM_64BIT_ARCH
    static inline void DoubleSizeDivide(unsigned long long &dividend_high, unsigned long long &dividend_low,
                                        const unsigned long long divisor) noexcept {
#ifdef _MSC_VER
        dividend_low = _udiv128(dividend_high, dividend_low, divisor, &dividend_high);
#else
        static constexpr unsigned int shift_size = (sizeof(long long) * 8U);

        __uint128_t dividend128 = dividend_high;
        dividend128 <<= shift_size;
        dividend128 |= dividend_low;
        dividend_high = static_cast<unsigned long long>(dividend128 % divisor);
        dividend128 /= divisor;
        dividend_low = static_cast<unsigned long long>(dividend128 & 0xFFFFFFFFFFFFFFFFULL);
#endif
    }
#endif

    inline Number_T_ DivideBy(const Number_T_ divisor) noexcept {
        Number_T_    remainder = 0;
        unsigned int index     = (index_ + 1U);

        do {
            --index;

            if ((remainder != Number_T_{0}) || (big_int_[index] != Number_T_{0})) {
                DoubleSizeDivide(remainder, big_int_[index], divisor);
            }
        } while (index != 0U);

        index_ -= ((index_ > 0U) && (big_int_[index_] == 0U));

        return remainder;
    }
    ////////////////////////////////////////////////////
    inline void ShiftRight(const unsigned int offset) noexcept {
        shiftRight(&(big_int_[0U]), index_, offset);
    }

    inline void ShiftLeft(const unsigned int offset) noexcept {
        shiftLeft(&(big_int_[0U]), index_, offset);
    }
    ////////////////////////////////////////////////////
    inline Number_T_ GetNumber() const noexcept {
        return big_int_[0];
    }

    inline Number_T_ const &GetBucket(const unsigned int index) const noexcept {
        return big_int_[index];
    }

    inline Number_T_ &GetBucket(const unsigned int index) noexcept {
        return big_int_[index];
    }

    inline unsigned int GetIndex() const noexcept {
        return index_;
    }

    inline bool IsBig() const noexcept {
        return (index_ != 0U);
    }

    inline bool NotZero() const noexcept {
        return (*this != Number_T_{0});
    }

    inline bool IsZero() const noexcept {
        return (*this == Number_T_{0});
    }

    static constexpr unsigned int Size     = (sizeof(Number_T_) * 8U);
    static constexpr unsigned int MaxIndex = ((Width_T_ / Size) - 1U);

  private:
    static void shiftRight(Number_T_ *b_int, unsigned int &length, unsigned int offset) noexcept {
        unsigned int index = 0U;

        while (offset >= Size) {
            while (index < length) {
                b_int[index] = b_int[index + 1U];
                ++index;
            }

            b_int[length] = Number_T_{0};

            if (length != 0U) {
                --length;
            }

            index = 0U;
            offset -= Size;
        }

        if (offset != 0U) {
            const unsigned int shift_size = (Size - offset);

            b_int[0U] >>= offset;

            while (index <= length) {
                b_int[index] |= (b_int[index + 1U] << shift_size);
                ++index;
                b_int[index] >>= offset;
            }

            if (length != 0U) {
                length -= (b_int[length] == 0U);
            }
        }
    }

    static void shiftLeft(Number_T_ *b_int, unsigned int &length, unsigned int offset) noexcept {
        unsigned int index = length;
        Number_T_    carry;

        while (offset >= Size) {
            if ((b_int[index] != 0U) && (length < MaxIndex)) {
                ++length;
                b_int[length] = b_int[index];
            }

            while (index != 0U) {
                b_int[index] = b_int[index - 1U];
                --index;
            }

            b_int[0U] = Number_T_{0};
            index     = length;
            offset -= Size;
        }

        if (offset != 0U) {
            const unsigned int shift_size = (Size - offset);

            carry = (b_int[index] >> shift_size);
            b_int[index] <<= offset;

            if ((carry != 0U) && (length < MaxIndex)) {
                ++length;
                b_int[length] = carry;
            }

            while (index != 0U) {
                carry = (b_int[index - 1U] >> shift_size);
                b_int[index] |= carry;
                --index;
                b_int[index] <<= offset;
            }
        }
    }

    Number_T_    big_int_[MaxIndex + 1U]{Number_T_{0}};
    unsigned int index_{0};
};

} // namespace Qentem

#endif
