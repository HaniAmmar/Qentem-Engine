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

template <typename Number_T_, SizeT32>
struct DoubleSize {};

template <typename Number_T_, SizeT32 Width_T_>
struct BigInt {
    using NumberType_ = Number_T_;

    enum class BigIntOperation : unsigned char {
        // Internal use only
        Set = 0, // =
        Or,      // |
        And,     // &
        Add,     // +
        Subtract // -
    };

    BigInt() noexcept                          = default;
    BigInt(BigInt &&) noexcept                 = default;
    BigInt(const BigInt &) noexcept            = default;
    BigInt &operator=(BigInt &&) noexcept      = default;
    BigInt &operator=(const BigInt &) noexcept = default;
    ~BigInt() noexcept                         = default;

    template <typename N_Number_T_>
    inline BigInt(const N_Number_T_ number) noexcept {
        multiOperation<BigIntOperation::Set>(number);
    }

    template <typename N_Number_T_>
    inline BigInt &operator=(const N_Number_T_ number) noexcept {
        while (index_id_ != Number_T_{0}) {
            big_int_[index_id_] = Number_T_{0};
            --index_id_;
        }

        multiOperation<BigIntOperation::Set>(number);

        return *this;
    }

    template <typename N_Number_T_>
    inline explicit operator N_Number_T_() const noexcept {
        constexpr SizeT32 n_size = (sizeof(N_Number_T_) * 8U);

        if constexpr (n_size == BitSize()) {
            return big_int_[0];
        } else if constexpr (n_size < BitSize()) {
            return N_Number_T_(big_int_[0]);
        } else {
            N_Number_T_ num   = 0;
            SizeT32     count = (n_size / BitSize());

            while ((--count != SizeT32{0}) && (count >= index_id_)) {
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

    inline void operator<<=(const SizeT32 number) noexcept {
        ShiftLeft(number);
    }

    inline void operator>>=(const SizeT32 number) noexcept {
        ShiftRight(number);
    }

    template <typename N_Number_T_>
    inline void operator|=(const N_Number_T_ number) noexcept {
        multiOperation<BigIntOperation::Or>(number);
    }

    template <typename N_Number_T_>
    inline void operator&=(const N_Number_T_ number) noexcept {
        multiOperation<BigIntOperation::And>(number);
    }

    template <typename N_Number_T_>
    inline void operator+=(const N_Number_T_ number) noexcept {
        multiOperation<BigIntOperation::Add>(number);
    }

    inline void operator-=(const Number_T_ number) noexcept {
        multiOperation<BigIntOperation::Subtract>(number);
    }

    inline void operator*=(const Number_T_ number) noexcept {
        Multiply(number);
    }

    inline void operator/=(const Number_T_ number) noexcept {
        Divide(number);
    }
    ////////////////////////////////////////////////////
    void Add(Number_T_ number, Number_T_ index = 0) noexcept {
        if (number != Number_T_{0}) {
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
        if (number != Number_T_{0}) {
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
    inline void Multiply(Number_T_ multiplier) noexcept {
        Number_T_ index = (index_id_ + Number_T_{1});

        do {
            --index;
            Add(DoubleSize<Number_T_, BitSize()>::Multiply(big_int_[index], multiplier), (index + Number_T_{1}));
        } while (index != Number_T_{0});
    }
    ////////////////////////////////////////////////////
    inline Number_T_ Divide(const Number_T_ divisor) noexcept {
        Number_T_ index     = index_id_;
        Number_T_ remainder = (big_int_[index_id_] % divisor);
        big_int_[index_id_] /= divisor;

        const SizeT32 initial_shift = [=]() noexcept -> SizeT32 {
            static constexpr SizeT32 max_index = (BitSize() - 1U);

            if constexpr (BitSize() == 64U) {
                return (max_index - Platform::FindLastBit(divisor));
            } else {
                return 0U;
            }
        }();

        while (index != Number_T_{0}) {
            --index;

            if constexpr (BitSize() == 64U) {
                DoubleSize<Number_T_, BitSize()>::Divide(remainder, big_int_[index], divisor, initial_shift);
            } else {
                DoubleSize<Number_T_, BitSize()>::Divide(remainder, big_int_[index], divisor);
            }
        }

        index_id_ -= ((index_id_ > Number_T_{0}) && (big_int_[index_id_] == Number_T_{0}));

        return remainder;
    }
    ////////////////////////////////////////////////////
    inline void ShiftRight(SizeT32 offset) noexcept {
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
            const SizeT32 shift_size = (BitSize() - offset);

            big_int_[Number_T_{0}] >>= offset;

            while (index < index_id_) {
                big_int_[index] |= (big_int_[index + Number_T_{1}] << shift_size);
                ++index;
                big_int_[index] >>= offset;
            }

            index_id_ -= ((index_id_ != Number_T_{0}) && (big_int_[index_id_] == Number_T_{0}));
        }
    }

    inline void ShiftLeft(SizeT32 offset) noexcept {
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
            const SizeT32 shift_size = (BitSize() - offset);

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
    inline SizeT32 FindFirstBit() const noexcept {
        Number_T_ index = Number_T_{0};

        while ((big_int_[index] == Number_T_{0}) && (index <= index_id_)) {
            ++index;
        }

        return (Platform::FindFirstBit(big_int_[index_id_]) + (index * BitSize()));
    }

    inline SizeT32 FindLastBit() const noexcept {
        return (Platform::FindLastBit(big_int_[index_id_]) + SizeT32(index_id_ * BitSize()));
    }
    ////////////////////////////////////////////////////
    inline Number_T_ Number() const noexcept {
        return big_int_[0U];
    }

    inline Number_T_ const *Bucket(const SizeT32 index) const noexcept {
        if (index <= index_id_) {
            return (big_int_ + index);
        }

        return nullptr;
    }

    inline Number_T_ *Bucket(const SizeT32 index) noexcept {
        if (index <= index_id_) {
            return (big_int_ + index);
        }

        return nullptr;
    }

    inline Number_T_ Index() const noexcept {
        return index_id_;
    }

    inline static constexpr SizeT32 MaxIndex() noexcept {
        constexpr SizeT32 size      = (TotalBits() / BitSize());
        constexpr SizeT32 max_index = (((BitSize() * size) == TotalBits()) ? size : size + SizeT32{1});

        return max_index;
    }

    inline static constexpr SizeT32 BitSize() noexcept {
        return (SizeOfType() * 8U);
    }

    inline static constexpr SizeT32 TotalBits() noexcept {
        return Width_T_;
    }

    inline static constexpr SizeT32 SizeOfType() noexcept {
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

    inline void SetIndex(Number_T_ id) noexcept {
        index_id_ = id;
    }

    inline Number_T_ *Storage() noexcept {
        return big_int_;
    }

  private:
    Number_T_ big_int_[MaxIndex() + Number_T_{1}]{0};
    Number_T_ index_id_{0};

    template <BigIntOperation Operation, typename N_Number_T_>
    inline void multiOperation(N_Number_T_ number) noexcept {
        constexpr SizeT32 n_size = ((sizeof(N_Number_T_) * 8U) / BitSize());

        if constexpr (n_size > 1U) {
            if constexpr (Operation == BigIntOperation::Add) {
                Add(Number_T_(number));
            } else if constexpr (Operation == BigIntOperation::Subtract) {
                Subtract(Number_T_(number));
            } else if constexpr (Operation == BigIntOperation::Or) {
                big_int_[0U] |= Number_T_(number);
            } else if constexpr (Operation == BigIntOperation::And) {
                big_int_[0U] &= Number_T_(number);
            } else {
                big_int_[0U] = Number_T_(number);
            }

            number >>= BitSize();

            while (number != N_Number_T_{0}) {
                ++index_id_;

                if constexpr (Operation == BigIntOperation::Add) {
                    Add(Number_T_(number), index_id_);
                } else if constexpr (Operation == BigIntOperation::Subtract) {
                    Subtract(Number_T_(number), index_id_);
                } else if constexpr (Operation == BigIntOperation::Or) {
                    big_int_[index_id_] |= Number_T_(number);
                } else if constexpr (Operation == BigIntOperation::And) {
                    big_int_[index_id_] &= Number_T_(number);
                } else {
                    big_int_[index_id_] = Number_T_(number);
                }

                number >>= BitSize();
            }
        } else {
            if constexpr (Operation == BigIntOperation::Add) {
                Add(number);
            } else if constexpr (Operation == BigIntOperation::Subtract) {
                Subtract(number);
            } else if constexpr (Operation == BigIntOperation::Or) {
                big_int_[0U] |= number;
            } else if constexpr (Operation == BigIntOperation::And) {
                big_int_[0U] &= number;
            } else {
                big_int_[0U] = number;
            }
        }
    }
};
////////////////////////////////////////////////////
template <typename Number_T_>
struct DoubleSize<Number_T_, 8U> {
    static constexpr SizeT32 shift = 8U;

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
    static constexpr SizeT32 shift = 16U;

    inline static void Divide(Number_T_ &dividend_high, Number_T_ &dividend_low, const Number_T_ divisor) noexcept {
        SizeT32 dividend32 = dividend_high;
        dividend32 <<= shift;
        dividend32 |= dividend_low;
        dividend_high = Number_T_(dividend32 % divisor);
        dividend32 /= divisor;
        dividend_low = Number_T_(dividend32);
    }

    inline static Number_T_ Multiply(Number_T_ &number, const Number_T_ multiplier) noexcept {
        SizeT32 number32 = number;
        number32 *= multiplier;
        number = Number_T_(number32);

        return Number_T_(number32 >> shift);
    }
};
////////////////////////////////////////////////////
template <typename Number_T_>
struct DoubleSize<Number_T_, 32U> {
    static constexpr SizeT32 shift = 32U;

    inline static void Divide(Number_T_ &dividend_high, Number_T_ &dividend_low, const Number_T_ divisor) noexcept {
        SizeT64 dividend64 = dividend_high;
        dividend64 <<= shift;
        dividend64 |= dividend_low;
        dividend_high = Number_T_(dividend64 % divisor);
        dividend64 /= divisor;
        dividend_low = Number_T_(dividend64);
    }

    inline static Number_T_ Multiply(Number_T_ &number, const Number_T_ multiplier) noexcept {
        SizeT64 number64 = number;
        number64 *= multiplier;
        number = Number_T_(number64);

        return Number_T_(number64 >> shift);
    }
};
////////////////////////////////////////////////////
template <typename Number_T_>
struct DoubleSize<Number_T_, 64U> {
    inline static void Divide(Number_T_ &dividend_high, Number_T_ &dividend_low, const Number_T_ divisor,
                              const SizeT32 initial_shift) noexcept {
        const Number_T_ carry = (dividend_low % divisor);
        dividend_low /= divisor;
        // -----------------------
        const Number_T_ divisor_shifted = (divisor << initial_shift);
        // -----------------------
        const Number_T_ divisor_low  = (divisor_shifted >> shift);
        const Number_T_ divisor_high = (divisor_shifted & mask);
        // -----------------------
        dividend_high <<= initial_shift;
        // -----------------------
        Number_T_ quotient = (dividend_high / divisor_low);
        dividend_high %= divisor_low;
        Number_T_ reminder = (quotient * divisor_high);

        dividend_high <<= shift;

        if (dividend_high < reminder) {
            --quotient;

            if ((reminder - dividend_high) > divisor_shifted) {
                --quotient;
                reminder -= divisor_shifted;
            }

            reminder -= divisor_shifted;
        }

        dividend_high -= reminder;
        // -----------------------
        quotient <<= shift;
        dividend_low += quotient;
        // -----------------------
        quotient = (dividend_high / divisor_low);
        dividend_high %= divisor_low;
        reminder = (quotient * divisor_high);

        dividend_high <<= shift;

        if (dividend_high < reminder) {
            --quotient;

            if ((reminder - dividend_high) > divisor_shifted) {
                --quotient;
                reminder -= divisor_shifted;
            }

            reminder -= divisor_shifted;
        }

        dividend_high -= reminder;
        // -----------------------
        dividend_low += quotient;
        // -----------------------
        dividend_high >>= initial_shift;
        // -----------------------
        const Number_T_ original_dividend_high = dividend_high;
        dividend_high += carry;
        // -----------------------
        if (original_dividend_high > dividend_high) {
            // Overflow
            constexpr Number_T_ overflow_dividend = (Number_T_{1} << (width - 1U));

            dividend_high += ((overflow_dividend % (divisor >> 1U)) << 1U);
            ++dividend_low;
        }

        if (dividend_high >= divisor) {
            dividend_high -= divisor;
            ++dividend_low;
        }
    }

    inline static Number_T_ Multiply(Number_T_ &number, Number_T_ multiplier) noexcept {
        const Number_T_ number_low     = (number & mask);
        Number_T_       number_high    = number;
        Number_T_       multiplier_low = (multiplier & mask);

        number = (number_low * multiplier_low);

        number_high >>= shift;
        multiplier_low *= number_high;
        multiplier_low += (number >> shift);
        number &= mask;

        multiplier >>= shift;
        number_high *= multiplier;
        number_high += (multiplier_low >> shift);

        multiplier_low &= mask;
        multiplier_low += (number_low * multiplier);

        number |= (multiplier_low << shift);
        multiplier_low >>= shift;
        number_high += multiplier_low;

        return number_high;
    }

  private:
    static constexpr SizeT32   width = (sizeof(Number_T_) * 8U);
    static constexpr SizeT32   shift = (width / 2U);
    static constexpr Number_T_ mask  = (~(Number_T_{0}) >> shift);
};

} // namespace Qentem

#endif
