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

template <typename Number_T_, unsigned int Size_>
struct DoubleSize {};

template <typename Number_T_, unsigned int Width_T_>
struct BigInt {
    BigInt() noexcept                          = default;
    BigInt(BigInt &&) noexcept                 = default;
    BigInt(const BigInt &) noexcept            = default;
    BigInt &operator=(BigInt &&) noexcept      = default;
    BigInt &operator=(const BigInt &) noexcept = default;
    ~BigInt() noexcept                         = default;

    template <typename N_Number_T_>
    explicit BigInt(const N_Number_T_ number) noexcept {
        set(number);
    }

    template <typename N_Number_T_>
    BigInt &operator=(const Number_T_ number) noexcept {
        while (index_ != 0U) {
            big_int_[index_] = Number_T_{0};
            --index_;
        }

        set(number);

        return *this;
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
    void Add(Number_T_ number, unsigned int index = 0U) noexcept {
        if (number != 0) {
            while (index <= MaxIndex) {
                const Number_T_ tmp = big_int_[index];
                big_int_[index] += number;

                if (big_int_[index] > tmp) {
                    break;
                }

                // Overflow.
                number = Number_T_{1};
                ++index;
            }

            if ((index > index_) && (index <= MaxIndex)) {
                ++index_;
            }
        }
    }

    inline void Subtract(Number_T_ number, unsigned int index = 0U) noexcept {
        if (number != 0) {
            while (index <= MaxIndex) {
                const Number_T_ tmp = big_int_[index];
                big_int_[index] -= number;

                if (big_int_[index] < tmp) {
                    break;
                }

                // Overflow.
                number = Number_T_{1};
                ++index;
            }

            if ((index > 0U) && (big_int_[index_] == 0U)) {
                --index_;
            }
        }
    }
    ////////////////////////////////////////////////////
    inline void MultiplyBy(Number_T_ multiplier) noexcept {
        unsigned int index = (index_ + 1U);

        do {
            --index;

            if (big_int_[index] != 0U) {
                Add(DoubleSize<Number_T_, Size>::Multiply(big_int_[index], multiplier), (index + 1U));
            }
        } while (index != 0U);
    }
    ////////////////////////////////////////////////////
    inline Number_T_ DivideBy(const Number_T_ divisor) noexcept {
        Number_T_    remainder = 0;
        unsigned int index     = (index_ + 1U);

        do {
            --index;

            if ((remainder != Number_T_{0}) || (big_int_[index] != Number_T_{0})) {
                DoubleSize<Number_T_, Size>::Divide(remainder, big_int_[index], divisor);
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

    void set(const Number_T_ number) noexcept {
        big_int_[0U] = number;
    }

    template <typename N_Number_T_>
    void set(N_Number_T_ number) noexcept {
        static constexpr unsigned int b_size = ((sizeof(N_Number_T_) * 8U) / Size);

        big_int_[0U] = static_cast<Number_T_>(number);

        if constexpr (b_size > 1U) {
            number >>= Size;

            while (number != N_Number_T_{0}) {
                ++index_;
                big_int_[index_] = static_cast<Number_T_>(number);
                number >>= Size;
            }
        }
    }

    Number_T_    big_int_[MaxIndex + 1U]{Number_T_{0}};
    unsigned int index_{0};
};
////////////////////////////////////////////////////
template <typename Number_T_>
struct DoubleSize<Number_T_, 8U> {
    static inline void Divide(Number_T_ &dividend_high, Number_T_ &dividend_low, const Number_T_ divisor) noexcept {
        unsigned short dividend16 = dividend_high;
        dividend16 <<= 8U;
        dividend16 |= dividend_low;
        dividend_high = ((dividend16 % divisor) & 0xFFU);
        dividend16 /= divisor;
        dividend_low = (dividend16 & 0xFFU);
    }

    inline static Number_T_ Multiply(Number_T_ &number, const Number_T_ multiplier) noexcept {
        unsigned short number16 = number;
        number16 *= multiplier;
        number = (number16 & 0xFFU);

        return (number16 >> 8U);
    }
};
////////////////////////////////////////////////////
template <typename Number_T_>
struct DoubleSize<Number_T_, 16U> {
    static inline void Divide(Number_T_ &dividend_high, Number_T_ &dividend_low, const Number_T_ divisor) noexcept {
        unsigned int dividend32 = dividend_high;
        dividend32 <<= 16U;
        dividend32 |= dividend_low;
        dividend_high = ((dividend32 % divisor) & 0xFFFFU);
        dividend32 /= divisor;
        dividend_low = (dividend32 & 0xFFFFU);
    }

    inline static Number_T_ Multiply(Number_T_ &number, const Number_T_ multiplier) noexcept {
        unsigned int number32 = number;
        number32 *= multiplier;
        number = (number32 & 0xFFFFU);

        return ((number32 >> 16U) & 0xFFFFU);
    }
};
////////////////////////////////////////////////////
template <typename Number_T_>
struct DoubleSize<Number_T_, 32U> {
    static inline void Divide(Number_T_ &dividend_high, Number_T_ &dividend_low, const Number_T_ divisor) noexcept {
        unsigned long long dividend64 = dividend_high;
        dividend64 <<= 32U;
        dividend64 |= dividend_low;
        dividend_high = ((dividend64 % divisor) & 0xFFFFFFFFU);
        dividend64 /= divisor;
        dividend_low = (dividend64 & 0xFFFFFFFFU);
    }

    inline static Number_T_ Multiply(Number_T_ &number, const Number_T_ multiplier) noexcept {
        unsigned long long number64 = number;
        number64 *= multiplier;
        number = (number64 & 0xFFFFFFFFU);

        return ((number64 >> 32U) & 0xFFFFFFFFU);
    }
};
////////////////////////////////////////////////////
#ifdef QENTEM_64BIT_ARCH
template <typename Number_T_>
struct DoubleSize<Number_T_, 64U> {
    static inline void Divide(Number_T_ &dividend_high, Number_T_ &dividend_low, const Number_T_ divisor) noexcept {
#ifdef _MSC_VER
        dividend_low = _udiv128(dividend_high, dividend_low, divisor, &dividend_high);
#else
        __uint128_t dividend128 = dividend_high;
        dividend128 <<= 64U;
        dividend128 |= dividend_low;
        dividend_high = ((dividend128 % divisor) & 0xFFFFFFFFFFFFFFFFULL);
        dividend128 /= divisor;
        dividend_low          = (dividend128 & 0xFFFFFFFFFFFFFFFFULL);
#endif
    }

    inline static Number_T_ Multiply(Number_T_ &number, const Number_T_ multiplier) noexcept {
#ifdef _MSC_VER
        Number_T_ carry = 0;
        number          = _umul128(multiplier, number, &carry);

        return carry;
#else
        __uint128_t number128 = number;
        number128 *= multiplier;
        number = (number128 & 0xFFFFFFFFFFFFFFFFULL);

        return ((number128 >> 64U) & 0xFFFFFFFFFFFFFFFFULL);
#endif
    }
};
#endif

} // namespace Qentem

#endif
