/*
 * Copyright (c) 2024 Hani Ammar
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

#ifndef QENTEM_BIG_INT_H
#define QENTEM_BIG_INT_H

#include "Platform.hpp"

namespace Qentem {
/*
 * For TRUE arbitrary-precision arithmetic.
 */

// Width_T is the needed number of bits:
// 2 64bit int = 128 bits
// 6 64bit int = 384 bits
// 16 64bit int = 1024 bits
// (number of variables) * sizeof(variable) * 8 = Width_T

template <typename Number_T, SizeT32>
struct DoubleSize {};

template <typename Number_T, SizeT32 Width_T>
struct BigInt {
    using NumberType = Number_T;

    enum struct BigIntOperation : SizeT8 {
        // Internal use only
        Set = 0, // =
        Or,      // |
        And,     // &
        Add,     // +
        Subtract // -
    };

    BigInt() noexcept  = default;
    ~BigInt() noexcept = default;

    BigInt(BigInt &&src) noexcept : index_{src.index_} {
        SizeT32 index = 0U;

        while (index <= index_) {
            storage_[index] = src.storage_[index];
            ++index;
        }

        src.Clear();
    }

    BigInt(const BigInt &src) noexcept : index_{src.index_} {
        SizeT32 index = 0U;

        while (index <= index_) {
            storage_[index] = src.storage_[index];
            ++index;
        }
    }

    template <typename N_Number_T>
    inline BigInt(const N_Number_T number) noexcept {
        doOperation<BigIntOperation::Set>(number);
    }

    template <typename N_Number_T>
    inline BigInt &operator=(const N_Number_T number) noexcept {
        SizeT32 index = index_;
        doOperation<BigIntOperation::Set>(number);

        while (index > index_) {
            storage_[index] = 0;
            --index;
        }

        return *this;
    }

    BigInt &operator=(BigInt &&src) noexcept {
        if (this != &src) {
            copy(src);
            src.Clear();
        }

        return *this;
    }

    BigInt &operator=(const BigInt &src) noexcept {
        if (this != &src) {
            copy(src);
        }

        return *this;
    }

    template <typename N_Number_T>
    inline explicit operator N_Number_T() const noexcept {
        constexpr SizeT32 n_width         = (sizeof(N_Number_T) * 8U);
        constexpr bool    is_same_size    = (n_width == TypeWidth());
        constexpr bool    is_smaller_size = (n_width < TypeWidth());

        if QENTEM_CONST_EXPRESSION (is_smaller_size || is_same_size) {
            return N_Number_T(storage_[0]);
        } else {
            constexpr SizeT32 max_index = ((n_width / TypeWidth()) - 1U);
            SizeT32           index     = ((max_index <= index_) ? max_index : index_);
            N_Number_T        num{0};

            while (index > 0) {
                num |= storage_[index];
                num <<= TypeWidth();
                --index;
            }

            num |= storage_[0];
            return num;
        }
    }

    inline friend bool operator<(const BigInt &out, const Number_T number) noexcept {
        return (((out.index_ == 0U) && (out.storage_[0U] < number)));
    }

    inline friend bool operator<(const Number_T number, const BigInt &out) noexcept {
        return (out > number);
    }

    inline friend bool operator<=(const BigInt &out, const Number_T number) noexcept {
        return (((out.index_ == 0U) && (out.storage_[0U] <= number)));
    }

    inline friend bool operator<=(const Number_T number, const BigInt &out) noexcept {
        return (out >= number);
    }

    inline friend bool operator>(const BigInt &out, const Number_T number) noexcept {
        return (((out.index_ != 0U) || (out.storage_[0U] > number)));
    }

    inline friend bool operator>(const Number_T number, const BigInt &out) noexcept {
        return (out < number);
    }

    inline friend bool operator>=(const BigInt &out, const Number_T number) noexcept {
        return (((out.index_ != 0U) || (out.storage_[0U] >= number)));
    }

    inline friend bool operator>=(const Number_T number, const BigInt &out) noexcept {
        return (out <= number);
    }

    inline friend bool operator==(const BigInt &out, const Number_T number) noexcept {
        return (((out.index_ == 0U) && (out.storage_[0U] == number)));
    }

    inline friend bool operator==(const Number_T number, const BigInt &out) noexcept {
        return (out == number);
    }

    inline friend bool operator!=(const BigInt &out, const Number_T number) noexcept {
        return (((out.index_ != 0U) || (out.storage_[0U] != number)));
    }

    inline friend bool operator!=(const Number_T number, const BigInt &out) noexcept {
        return (out != number);
    }

    inline void operator<<=(const SizeT32 number) noexcept {
        ShiftLeft(number);
    }

    inline void operator>>=(const SizeT32 number) noexcept {
        ShiftRight(number);
    }

    template <typename N_Number_T>
    inline void operator|=(const N_Number_T number) noexcept {
        doOperation<BigIntOperation::Or>(number);
    }

    template <typename N_Number_T>
    inline void operator&=(const N_Number_T number) noexcept {
        doOperation<BigIntOperation::And>(number);
    }

    template <typename N_Number_T>
    inline void operator+=(const N_Number_T number) noexcept {
        doOperation<BigIntOperation::Add>(number);
    }

    template <typename N_Number_T>
    inline void operator-=(const N_Number_T number) noexcept {
        doOperation<BigIntOperation::Subtract>(number);
    }

    inline void operator*=(const Number_T number) noexcept {
        Multiply(number);
    }

    inline void operator/=(const Number_T number) noexcept {
        Divide(number);
    }
    ////////////////////////////////////////////////////
    void Add(Number_T number, SizeT32 index = 0U) noexcept {
        if (number != 0) {
            while (index <= MaxIndex()) {
                const Number_T tmp = storage_[index];
                storage_[index] += number;

                if (storage_[index] > tmp) {
                    break;
                }

                // Overflow.
                number = Number_T{1};
                ++index;
            }

            if (index > MaxIndex()) {
                index_ = 0;
            } else if (index > index_) {
                index_ = index;
            }
        }
    }

    inline void Subtract(Number_T number, SizeT32 index = 0U) noexcept {
        if (number != 0) {
            while (index <= MaxIndex()) {
                const Number_T tmp = storage_[index];
                storage_[index] -= number;

                if (storage_[index] < tmp) {
                    break;
                }

                // Overflow.
                number = Number_T{1};
                ++index;
            }

            if (index > MaxIndex()) {
                index_ = MaxIndex();
            } else if (index >= index_) {
                while ((index_ > 0U) && (storage_[index_] == 0)) {
                    --index_;
                }
            }
        }
    }
    ////////////////////////////////////////////////////
    inline void Multiply(Number_T multiplier) noexcept {
        SizeT32 index = index_;
        ++index;

        do {
            --index;
            Add(DoubleSize<Number_T, TypeWidth()>::Multiply(storage_[index], multiplier), (index + 1U));
        } while (index != 0U);
    }
    ////////////////////////////////////////////////////
    inline Number_T Divide(const Number_T divisor) noexcept {
        constexpr bool is_size_64b = (TypeWidth() == 64U);
        SizeT32        index       = index_;
        Number_T       remainder   = (storage_[index_] % divisor);
        storage_[index_] /= divisor;

        const SizeT32 initial_shift = [=]() noexcept -> SizeT32 {
            if (is_size_64b) {
                return ((TypeWidth() - 1U) - Platform::FindLastBit(divisor));
            }

            return 0U;
        }();

        while (index != 0U) {
            --index;
            DoubleSize<Number_T, TypeWidth()>::Divide(remainder, storage_[index], divisor, initial_shift);
        }

        index_ -= SizeT32((index_ > 0U) && (storage_[index_] == 0));

        return remainder;
    }
    ////////////////////////////////////////////////////
    inline void ShiftRight(SizeT32 offset) noexcept {
        if (offset >= TypeWidth()) {
            SizeT32 move = (offset / TypeWidth());
            offset -= (move * TypeWidth());

            if (move > index_) {
                Clear();
                return;
            }

            SizeT32 index = 0U;
            SizeT32 next  = (index + move);

            do {
                storage_[index] = storage_[next];
                ++index;
                ++next;
            } while (next <= index_);

            do {
                storage_[index_] = 0;
                --index_;
                --move;
            } while (move != 0U);
        }

        if (offset != 0U) {
            SizeT32       index      = 0U;
            const SizeT32 shift_size = (TypeWidth() - offset);

            storage_[0U] >>= offset;

            while (index < index_) {
                storage_[index] |= (storage_[index + 1U] << shift_size);
                ++index;
                storage_[index] >>= offset;
            }

            index_ -= SizeT32((index_ != 0U) && (storage_[index_] == 0));
        }
    }

    inline void ShiftLeft(SizeT32 offset) noexcept {
        if (offset >= TypeWidth()) {
            SizeT32 move = (offset / TypeWidth());
            offset -= (move * TypeWidth());

            SizeT32 index = index_;
            index += move;

            if (index > MaxIndex()) {
                const SizeT32 diff = (index - MaxIndex());

                if (diff <= index_) {
                    index_ -= diff;
                    index = MaxIndex();
                } else {
                    Clear();
                    return;
                }
            }

            storage_[index_ + move] = storage_[index_];

            while (index_ != 0U) {
                --index_;
                storage_[index_ + move] = storage_[index_];
            }

            do {
                --move;
                storage_[move] = 0;
            } while (move != 0U);

            while (storage_[index] == 0) {
                --index;
            }

            index_ = index;
        }

        if (offset != 0U) {
            SizeT32        index      = index_;
            const SizeT32  shift_size = (TypeWidth() - offset);
            const Number_T carry      = (storage_[index] >> shift_size);

            storage_[index] <<= offset;

            if (index_ != MaxIndex()) {
                index_ += SizeT32(carry != 0);
                storage_[index_] |= carry;
            }

            while (index != 0U) {
                storage_[index] |= (storage_[index - 1U] >> shift_size);
                --index;
                storage_[index] <<= offset;
            }
        }
    }
    ////////////////////////////////////////////////////
    inline SizeT32 FindFirstBit() const noexcept {
        SizeT32 index = 0U;

        while ((storage_[index] == 0) && (index <= index_)) {
            ++index;
        }

        return (Platform::FindFirstBit(storage_[index_]) + (index * TypeWidth()));
    }

    inline SizeT32 FindLastBit() const noexcept {
        return (Platform::FindLastBit(storage_[index_]) + SizeT32(index_ * TypeWidth()));
    }
    ////////////////////////////////////////////////////
    inline Number_T Number() const noexcept {
        return storage_[0U];
    }

    inline void Clear() noexcept {
        while (index_ != 0U) {
            storage_[index_] = 0;
            --index_;
        }

        storage_[0U] = 0;
    }

    inline SizeT32 Index() const noexcept {
        return index_;
    }

    inline static constexpr SizeT32 MaxIndex() noexcept {
        return ((TotalBits() / TypeWidth()) - 1U);
    }

    inline static constexpr SizeT32 TypeWidth() noexcept {
        return (SizeOfType() * 8U);
    }

    inline static constexpr SizeT32 TotalBits() noexcept {
        return (((TypeWidth() * (Width_T / TypeWidth())) == Width_T)
                    ? Width_T
                    : ((TypeWidth() * (Width_T / TypeWidth())) + TypeWidth()));

        // "TypeWidth() * X" does not cancel "X / TypeWidth()"; Because this is computer science not math.
    }

    inline static constexpr SizeT32 SizeOfType() noexcept {
        return sizeof(Number_T);
    }

    inline bool IsBig() const noexcept {
        return (index_ != 0U);
    }

    inline bool NotZero() const noexcept {
        return (*this != 0);
    }

    inline bool IsZero() const noexcept {
        return (*this == 0);
    }

    inline void SetIndex(SizeT32 index) noexcept {
        index_ = index;
    }

    inline Number_T *Storage() noexcept {
        return storage_;
    }

    inline const Number_T *Storage() const noexcept {
        return storage_;
    }

  private:
    Number_T storage_[MaxIndex() + Number_T{1}]{0};
    SizeT32  index_{0};

    void copy(const BigInt &src) noexcept {
        SizeT32 index = 0U;

        while (index <= src.index_) {
            storage_[index] = src.storage_[index];
            ++index;
        }

        while (index_ > index) {
            storage_[index_] = 0;
            --index_;
        }

        index_ = src.index_;
    }

    template <BigIntOperation Operation>
    inline void doOperation(Number_T number) noexcept {
        switch (Operation) {
            case BigIntOperation::Add: {
                Add(number);
                break;
            }

            case BigIntOperation::Subtract: {
                Subtract(number);
                break;
            }

            case BigIntOperation::Or: {
                storage_[0U] |= number;
                break;
            }

            case BigIntOperation::And: {
                storage_[0U] &= number;
                index_ = 0U;
                break;
            }

            default: {
                storage_[0U] = number;
                index_       = 0U;
            }
        }
    }

    template <BigIntOperation Operation, typename N_Number_T>
    inline void doOperation(N_Number_T number) noexcept {
        constexpr bool is_bigger_size = (((sizeof(N_Number_T) * 8U) / TypeWidth()) > 1U);

        switch (Operation) {
            case BigIntOperation::Add: {
                Add(Number_T(number));
                break;
            }

            case BigIntOperation::Subtract: {
                Subtract(Number_T(number));
                break;
            }

            case BigIntOperation::Or: {
                storage_[0U] |= Number_T(number);
                break;
            }

            case BigIntOperation::And: {
                storage_[0U] &= Number_T(number);
                index_ = 0U;
                break;
            }

            default: {
                storage_[0U] = Number_T(number);
                index_       = 0U;
            }
        }

        if QENTEM_CONST_EXPRESSION (is_bigger_size) {
            SizeT32 index = 1U;
            number >>= TypeWidth();

            while (number != N_Number_T{0}) {
                switch (Operation) {
                    case BigIntOperation::Add: {
                        Add(Number_T(number), index);
                        break;
                    }

                    case BigIntOperation::Subtract: {
                        Subtract(Number_T(number), index);
                        break;
                    }

                    case BigIntOperation::Or: {
                        storage_[index] |= Number_T(number);

                        if (index > index_) {
                            index_ = index;
                        }

                        break;
                    }

                    case BigIntOperation::And: {
                        storage_[index] &= Number_T(number);

                        if (storage_[index] != Number_T(0)) {
                            index_ = index;
                        }

                        break;
                    }

                    default: {
                        storage_[index] = Number_T(number);
                        ++index_;
                    }
                }

                number >>= TypeWidth();
                ++index;
            }
        }
    }
};
////////////////////////////////////////////////////
template <typename Number_T>
struct DoubleSize<Number_T, 8U> {
    inline static void Divide(Number_T &dividend_high, Number_T &dividend_low, const Number_T divisor,
                              SizeT32) noexcept {
        SizeT16 dividend16{dividend_high};
        dividend16 <<= shift_;
        dividend16 |= dividend_low;
        dividend_high = Number_T(dividend16 % divisor);
        dividend16 /= divisor;
        dividend_low = Number_T(dividend16);
    }

    inline static Number_T Multiply(Number_T &number, const Number_T multiplier) noexcept {
        SizeT16 number16{number};
        number16 *= multiplier;
        number = Number_T(number16);

        return Number_T(number16 >> shift_);
    }

  private:
    static constexpr SizeT32 shift_ = 8U;
};
////////////////////////////////////////////////////
template <typename Number_T>
struct DoubleSize<Number_T, 16U> {
    inline static void Divide(Number_T &dividend_high, Number_T &dividend_low, const Number_T divisor,
                              SizeT32) noexcept {
        SizeT32 dividend32{dividend_high};
        dividend32 <<= shift_;
        dividend32 |= dividend_low;
        dividend_high = Number_T(dividend32 % divisor);
        dividend32 /= divisor;
        dividend_low = Number_T(dividend32);
    }

    inline static Number_T Multiply(Number_T &number, const Number_T multiplier) noexcept {
        SizeT32 number32{number};
        number32 *= multiplier;
        number = Number_T(number32);

        return Number_T(number32 >> shift_);
    }

  private:
    static constexpr SizeT32 shift_ = 16U;
};
////////////////////////////////////////////////////
template <typename Number_T>
struct DoubleSize<Number_T, 32U> {
    inline static void Divide(Number_T &dividend_high, Number_T &dividend_low, const Number_T divisor,
                              SizeT32) noexcept {
        SizeT64 dividend64{dividend_high};
        dividend64 <<= shift_;
        dividend64 |= dividend_low;
        dividend_high = Number_T(dividend64 % divisor);
        dividend64 /= divisor;
        dividend_low = Number_T(dividend64);
    }

    inline static Number_T Multiply(Number_T &number, const Number_T multiplier) noexcept {
        SizeT64 number64{number};
        number64 *= multiplier;
        number = Number_T(number64);

        return Number_T(number64 >> shift_);
    }

  private:
    static constexpr SizeT32 shift_ = 32U;
};
////////////////////////////////////////////////////
template <typename Number_T>
struct DoubleSize<Number_T, 64U> {
    inline static void Divide(Number_T &dividend_high, Number_T &dividend_low, const Number_T divisor,
                              const SizeT32 initial_shift) noexcept {
        const Number_T carry = (dividend_low % divisor);
        dividend_low /= divisor;
        // -----------------------
        const Number_T divisor_shifted = (divisor << initial_shift);
        // -----------------------
        const Number_T divisor_low  = (divisor_shifted >> shift_);
        const Number_T divisor_high = (divisor_shifted & mask_);
        // -----------------------
        dividend_high <<= initial_shift;
        // -----------------------
        Number_T quotient = (dividend_high / divisor_low);
        dividend_high %= divisor_low;
        Number_T remainder = (quotient * divisor_high);

        dividend_high <<= shift_;

        if (dividend_high < remainder) {
            --quotient;

            if ((remainder - dividend_high) > divisor_shifted) {
                --quotient;
                remainder -= divisor_shifted;
            }

            remainder -= divisor_shifted;
        }

        dividend_high -= remainder;
        // -----------------------
        quotient <<= shift_;
        dividend_low += quotient;
        // -----------------------
        quotient = (dividend_high / divisor_low);
        dividend_high %= divisor_low;
        remainder = (quotient * divisor_high);

        dividend_high <<= shift_;

        if (dividend_high < remainder) {
            --quotient;

            if ((remainder - dividend_high) > divisor_shifted) {
                --quotient;
                remainder -= divisor_shifted;
            }

            remainder -= divisor_shifted;
        }

        dividend_high -= remainder;
        // -----------------------
        dividend_low += quotient;
        // -----------------------
        dividend_high >>= initial_shift;
        // -----------------------
        const Number_T original_dividend_high = dividend_high;
        dividend_high += carry;
        // -----------------------
        if (original_dividend_high > dividend_high) {
            // Overflow
            constexpr Number_T overflow_dividend = (Number_T{1} << (width_ - 1U));

            dividend_high += ((overflow_dividend % (divisor >> 1U)) << 1U);
            ++dividend_low;
        }

        if (dividend_high >= divisor) {
            dividend_high -= divisor;
            ++dividend_low;
        }
    }

    inline static Number_T Multiply(Number_T &number, Number_T multiplier) noexcept {
        const Number_T number_low     = (number & mask_);
        Number_T       number_high    = number;
        Number_T       multiplier_low = (multiplier & mask_);

        number = (number_low * multiplier_low);

        number_high >>= shift_;
        multiplier_low *= number_high;
        multiplier_low += (number >> shift_);
        number &= mask_;

        multiplier >>= shift_;
        number_high *= multiplier;
        number_high += (multiplier_low >> shift_);

        multiplier_low &= mask_;
        multiplier_low += (number_low * multiplier);

        number |= (multiplier_low << shift_);
        multiplier_low >>= shift_;
        number_high += multiplier_low;

        return number_high;
    }

  private:
    static constexpr SizeT32  width_ = (sizeof(Number_T) * 8U);
    static constexpr SizeT32  shift_ = (width_ / 2U);
    static constexpr Number_T mask_  = (~(Number_T{0}) >> shift_);
};

} // namespace Qentem

#endif
