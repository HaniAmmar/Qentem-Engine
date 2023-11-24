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
// 6 64bit int = 384 bits
// 16 64bit int = 1024 bits
// (number of variables) * sizeof(variable) * 8 = Width_T_

template <typename Number_T_, unsigned int Size_>
struct DoubleSize {};

template <typename Number_T_, unsigned int Width_T_>
struct BigInt {
    using NumberType_ = Number_T_;

    BigInt() noexcept                          = default;
    BigInt(BigInt &&) noexcept                 = default;
    BigInt(const BigInt &) noexcept            = default;
    BigInt &operator=(BigInt &&) noexcept      = default;
    BigInt &operator=(const BigInt &) noexcept = default;
    ~BigInt() noexcept                         = default;

    template <typename N_Number_T_>
    inline BigInt(const N_Number_T_ number) noexcept {
        set(number);
    }

    template <typename N_Number_T_>
    inline BigInt &operator=(const N_Number_T_ number) noexcept {
        while (index_id_ != Number_T_{0}) {
            big_int_[index_id_] = Number_T_{0};
            --index_id_;
        }

        set(number);

        return *this;
    }

    inline explicit operator Number_T_() const {
        return big_int_[0];
    }

    template <typename N_Number_T_>
    inline explicit operator N_Number_T_() const {
        constexpr unsigned int n_size = (sizeof(N_Number_T_) * 8U);

        if constexpr (n_size == BitSize()) {
            return big_int_[0];
        } else if constexpr (n_size < BitSize()) {
            return N_Number_T_(big_int_[0]);
        } else {
            N_Number_T_  num   = 0;
            unsigned int count = (n_size / BitSize());

            while ((--count != 0) && (count >= index_id_)) {
                num |= big_int_[1];
                num <<= BitSize();
            }

            num |= big_int_[0];

            return num;
        }
    }

    inline friend bool operator<(const BigInt &out, const Number_T_ number) noexcept {
        return (((out.index_id_ == Number_T_{0}) && (out.big_int_[0U] < number)));
    }

    inline friend bool operator<(const Number_T_ number, const BigInt &out) noexcept {
        return (out > number);
    }

    inline friend bool operator<=(const BigInt &out, const Number_T_ number) noexcept {
        return (((out.index_id_ == Number_T_{0}) && (out.big_int_[0U] <= number)));
    }

    inline friend bool operator<=(const Number_T_ number, const BigInt &out) noexcept {
        return (out >= number);
    }

    inline friend bool operator>(const BigInt &out, const Number_T_ number) noexcept {
        return (((out.index_id_ != Number_T_{0}) || (out.big_int_[0U] > number)));
    }

    inline friend bool operator>(const Number_T_ number, const BigInt &out) noexcept {
        return (out < number);
    }

    inline friend bool operator>=(const BigInt &out, const Number_T_ number) noexcept {
        return (((out.index_id_ != Number_T_{0}) || (out.big_int_[0U] >= number)));
    }

    inline friend bool operator>=(const Number_T_ number, const BigInt &out) noexcept {
        return (out <= number);
    }

    inline friend bool operator==(const BigInt &out, const Number_T_ number) noexcept {
        return (((out.index_id_ == Number_T_{0}) && (out.big_int_[0U] == number)));
    }

    inline friend bool operator==(const Number_T_ number, const BigInt &out) noexcept {
        return (out == number);
    }

    inline friend bool operator!=(const BigInt &out, const Number_T_ number) noexcept {
        return (((out.index_id_ != Number_T_{0}) || (out.big_int_[0U] != number)));
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

    inline void operator*=(const Number_T_ number) noexcept {
        MultiplyBy(number);
    }

    inline void operator/=(const Number_T_ number) noexcept {
        DivideBy(number);
    }

    inline void operator+=(const Number_T_ number) noexcept {
        Add(number);
    }

    inline void operator-=(const Number_T_ number) noexcept {
        Subtract(number);
    }
    ////////////////////////////////////////////////////
    void Add(Number_T_ number, Number_T_ index = 0) noexcept {
        if (number != 0) {
            while (index <= MaxIndex()) {
                const Number_T_ tmp = big_int_[index];
                big_int_[index] += number;

                if (big_int_[index] > tmp) {
                    break;
                }

                // Overflow.
                number = Number_T_{1};
                ++index;
            }

            index_id_ += ((index > index_id_) && (index <= MaxIndex()));
        }
    }

    inline void Subtract(Number_T_ number, Number_T_ index = 0) noexcept {
        if (number != 0) {
            while (index <= MaxIndex()) {
                const Number_T_ tmp = big_int_[index];
                big_int_[index] -= number;

                if (big_int_[index] < tmp) {
                    break;
                }

                // Overflow.
                number = Number_T_{1};
                ++index;
            }

            index_id_ -= ((index > Number_T_{0}) && (big_int_[index_id_] == Number_T_{0}));
        }
    }
    ////////////////////////////////////////////////////
    inline void MultiplyBy(Number_T_ multiplier) noexcept {
        Number_T_ index = (index_id_ + Number_T_{1});

        do {
            --index;
            Add(DoubleSize<Number_T_, BitSize()>::Multiply(big_int_[index], multiplier), (index + Number_T_{1}));
        } while (index != Number_T_{0});
    }
    ////////////////////////////////////////////////////
    inline Number_T_ DivideBy(const Number_T_ divisor) noexcept {
        Number_T_ remainder = 0;
        Number_T_ index     = index_id_;

        remainder = (big_int_[index_id_] % divisor);
        big_int_[index_id_] /= divisor;

        while (index != Number_T_{0}) {
            --index;
            DoubleSize<Number_T_, BitSize()>::Divide(remainder, big_int_[index], divisor);
        }

        index_id_ -= ((index_id_ > Number_T_{0}) && (big_int_[index_id_] == Number_T_{0}));

        return remainder;
    }
    ////////////////////////////////////////////////////
    inline void ShiftRight(unsigned int offset) noexcept {
        Number_T_ index = 0U;

        while (offset >= BitSize()) {
            while (index < index_id_) {
                big_int_[index] = big_int_[index + Number_T_{1}];
                ++index;
            }

            big_int_[index_id_] = Number_T_{0};
            index_id_ -= (index_id_ != Number_T_{0});

            index = Number_T_{0};
            offset -= BitSize();
        }

        if (offset != Number_T_{0}) {
            const unsigned int shift_size = (BitSize() - offset);

            big_int_[Number_T_{0}] >>= offset;

            while (index < index_id_) {
                big_int_[index] |= (big_int_[index + Number_T_{1}] << shift_size);
                ++index;
                big_int_[index] >>= offset;
            }

            index_id_ -= ((index_id_ != Number_T_{0}) && (big_int_[index_id_] == Number_T_{0}));
        }
    }

    inline void ShiftLeft(unsigned int offset) noexcept {
        Number_T_ index = index_id_;

        while (offset >= BitSize()) {
            index_id_ += ((big_int_[index] != Number_T_{0}) && (index_id_ < MaxIndex()));
            big_int_[index_id_] = big_int_[index];

            while (index != Number_T_{0}) {
                big_int_[index] = big_int_[index - Number_T_{1}];
                --index;
            }

            big_int_[0U] = Number_T_{0};
            index        = index_id_;
            offset -= BitSize();
        }

        if (offset != Number_T_{0}) {
            const unsigned int shift_size = (BitSize() - offset);

            const Number_T_ carry = (big_int_[index] >> shift_size);
            big_int_[index] <<= offset;

            index_id_ += ((carry != Number_T_{0}) && (index_id_ < MaxIndex()));
            big_int_[index_id_] |= carry;

            while (index != Number_T_{0}) {
                big_int_[index] |= (big_int_[index - Number_T_{1}] >> shift_size);
                --index;
                big_int_[index] <<= offset;
            }
        }
    }
    ////////////////////////////////////////////////////
    inline unsigned int FindFirstBit() const noexcept {
        Number_T_ index = 0;

        while ((big_int_[index] == 0) && (index <= index_id_)) {
            ++index;
        }

        return (Platform::FindFirstBit(big_int_[index_id_]) + (index * BitSize()));
    }

    inline unsigned int FindLastBit() const noexcept {
        return (Platform::FindLastBit(big_int_[index_id_]) + (unsigned int)(index_id_ * BitSize()));
    }
    ////////////////////////////////////////////////////
    inline Number_T_ Number() const noexcept {
        return big_int_[0U];
    }

    inline Number_T_ const *Bucket(const unsigned int index) const noexcept {
        if (index <= index_id_) {
            return (big_int_ + index);
        }

        return nullptr;
    }

    inline Number_T_ *Bucket(const unsigned int index) noexcept {
        if (index <= index_id_) {
            return (big_int_ + index);
        }

        return nullptr;
    }

    inline Number_T_ Index() const noexcept {
        return index_id_;
    }

    inline static constexpr unsigned int MaxIndex() noexcept {
        return (AlignSize() - 1U);
    }

    inline static constexpr unsigned int BitSize() noexcept {
        return (SizeOfType() * 8U);
    }

    inline static constexpr unsigned int TotalBits() noexcept {
        return Width_T_;
    }

    inline static constexpr unsigned int SizeOfType() noexcept {
        return sizeof(Number_T_);
    }

    inline bool IsBig() const noexcept {
        return (index_id_ != Number_T_{0});
    }

    inline bool NotZero() const noexcept {
        return (*this != Number_T_{0});
    }

    inline bool IsZero() const noexcept {
        return (*this == Number_T_{0});
    }

    static constexpr unsigned int AlignSize() noexcept {
        return ((TotalBits() / BitSize()) +
                (unsigned int)(TotalBits() > (1U << Platform::FindLastBitConst(TotalBits()))));
    }

  private:
    Number_T_ big_int_[MaxIndex() + Number_T_{1}]{0};
    Number_T_ index_id_{0};

    inline void set(const Number_T_ number) noexcept {
        big_int_[0U] = number;
    }

    template <typename N_Number_T_>
    inline void set(N_Number_T_ number) noexcept {
        constexpr unsigned int n_size = ((sizeof(N_Number_T_) * 8U) / BitSize());

        big_int_[0U] = Number_T_(number);

        if constexpr (n_size > 1U) {
            number >>= BitSize();

            while (number != N_Number_T_{0}) {
                ++index_id_;
                big_int_[index_id_] = Number_T_(number);
                number >>= BitSize();
            }
        }
    }
};
////////////////////////////////////////////////////
template <typename Number_T_>
struct DoubleSize<Number_T_, 8U> {
    static constexpr unsigned int shift = 8U;

    inline static void Divide(Number_T_ &dividend_high, Number_T_ &dividend_low, const Number_T_ divisor) noexcept {
        unsigned short dividend16 = dividend_high;
        dividend16 <<= shift;
        dividend16 |= dividend_low;
        dividend_high = Number_T_(dividend16 % divisor);
        dividend16 /= divisor;
        dividend_low = Number_T_(dividend16);
    }

    inline static Number_T_ Multiply(Number_T_ &number, const Number_T_ multiplier) noexcept {
        unsigned short number16 = number;
        number16 *= multiplier;
        number = Number_T_(number16);

        return Number_T_(number16 >> shift);
    }
};
////////////////////////////////////////////////////
template <typename Number_T_>
struct DoubleSize<Number_T_, 16U> {
    static constexpr unsigned int shift = 16U;

    inline static void Divide(Number_T_ &dividend_high, Number_T_ &dividend_low, const Number_T_ divisor) noexcept {
        unsigned int dividend32 = dividend_high;
        dividend32 <<= shift;
        dividend32 |= dividend_low;
        dividend_high = Number_T_(dividend32 % divisor);
        dividend32 /= divisor;
        dividend_low = Number_T_(dividend32);
    }

    inline static Number_T_ Multiply(Number_T_ &number, const Number_T_ multiplier) noexcept {
        unsigned int number32 = number;
        number32 *= multiplier;
        number = Number_T_(number32);

        return Number_T_(number32 >> shift);
    }
};
////////////////////////////////////////////////////
template <typename Number_T_>
struct DoubleSize<Number_T_, 32U> {
    static constexpr unsigned int shift = 32U;

    inline static void Divide(Number_T_ &dividend_high, Number_T_ &dividend_low, const Number_T_ divisor) noexcept {
        unsigned long long dividend64 = dividend_high;
        dividend64 <<= shift;
        dividend64 |= dividend_low;
        dividend_high = Number_T_(dividend64 % divisor);
        dividend64 /= divisor;
        dividend_low = Number_T_(dividend64);
    }

    inline static Number_T_ Multiply(Number_T_ &number, const Number_T_ multiplier) noexcept {
        unsigned long long number64 = number;
        number64 *= multiplier;
        number = Number_T_(number64);

        return Number_T_(number64 >> shift);
    }
};
////////////////////////////////////////////////////
template <typename Number_T_>
struct DoubleSize<Number_T_, 64U> {
#ifdef QENTEM_64BIT_ARCH
    inline static void Divide(Number_T_ &dividend_high, Number_T_ &dividend_low, const Number_T_ divisor) noexcept {
#ifdef _MSC_VER
        dividend_low = _udiv128(dividend_high, dividend_low, divisor, &dividend_high);
#else
        __uint128_t dividend128 = dividend_high;
        dividend128 <<= 64U;
        dividend128 |= dividend_low;
        dividend_high = Number_T_(dividend128 % divisor);
        dividend128 /= divisor;
        dividend_low = Number_T_(dividend128);
#endif
    }
#endif

    inline static Number_T_ Multiply(Number_T_ &number, Number_T_ multiplier) noexcept {
        // constexpr unsigned int shift = (sizeof(Number_T_) * 4U);
        // constexpr Number_T_    mask  = (Number_T_{1} << shift) - Number_T_{1};
        constexpr unsigned int shift = 32U;
        constexpr Number_T_    mask  = 0xFFFFFFFFU;

        Number_T_       number_high    = (number >> shift);
        const Number_T_ number_low     = (number & mask);
        Number_T_       multiplier_low = (multiplier & mask);

        number = (number_low * multiplier_low);

        multiplier_low *= number_high;
        multiplier_low += (number >> shift);

        multiplier >>= shift;
        number_high *= multiplier;
        number_high += (multiplier_low >> shift);

        multiplier_low &= mask;
        multiplier_low += (number_low * multiplier);

        number &= mask;
        number |= (multiplier_low << shift);
        number_high += (multiplier_low >> shift);

        return number_high;
    }
};

} // namespace Qentem

#endif
