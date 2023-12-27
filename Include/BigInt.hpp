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

#ifndef QENTEM_BIG_INT_H
#define QENTEM_BIG_INT_H

namespace Qentem {

// For TRUE arbitrary-precision arithmetic.

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

    BigInt() noexcept                          = default;
    BigInt(BigInt &&) noexcept                 = default;
    BigInt(const BigInt &) noexcept            = default;
    BigInt &operator=(const BigInt &) noexcept = default;
    ~BigInt() noexcept                         = default;

    BigInt &operator=(BigInt &&bint) noexcept {
        SizeT32 index = 0U;

        while (index <= bint._index) {
            _storage[index] = bint._storage[index];
            ++index;
        }

        while (_index > index) {
            _storage[_index] = Number_T{0};
            --_index;
        }

        _index = bint._index;
        bint.Clear();

        return *this;
    }

    template <typename N_Number_T>
    inline BigInt(const N_Number_T number) noexcept {
        doOperation<BigIntOperation::Set>(number);
    }

    template <typename N_Number_T>
    inline BigInt &operator=(const N_Number_T number) noexcept {
        SizeT32 index = _index;
        doOperation<BigIntOperation::Set>(number);

        while (index > _index) {
            _storage[index] = Number_T{0};
            --index;
        }

        return *this;
    }

    template <typename N_Number_T>
    inline explicit operator N_Number_T() const noexcept {
        constexpr SizeT32 n_width         = (sizeof(N_Number_T) * 8U);
        constexpr bool    is_same_size    = (n_width == TypeWidth());
        constexpr bool    is_smaller_size = (n_width < TypeWidth());

        if (is_same_size) {
            return _storage[0];
        } else if (is_smaller_size) {
            return N_Number_T(_storage[0]);
        } else {
            N_Number_T num   = 0;
            SizeT32    count = (n_width / TypeWidth());

            while ((--count != 0U) && (count >= _index)) {
                num |= _storage[1];
                num <<= TypeWidth();
            }

            num |= _storage[0];

            return num;
        }
    }

    inline friend bool operator<(const BigInt &out, const Number_T number) noexcept {
        return (((out._index == 0U) && (out._storage[0U] < number)));
    }

    inline friend bool operator<(const Number_T number, const BigInt &out) noexcept {
        return (out > number);
    }

    inline friend bool operator<=(const BigInt &out, const Number_T number) noexcept {
        return (((out._index == 0U) && (out._storage[0U] <= number)));
    }

    inline friend bool operator<=(const Number_T number, const BigInt &out) noexcept {
        return (out >= number);
    }

    inline friend bool operator>(const BigInt &out, const Number_T number) noexcept {
        return (((out._index != 0U) || (out._storage[0U] > number)));
    }

    inline friend bool operator>(const Number_T number, const BigInt &out) noexcept {
        return (out < number);
    }

    inline friend bool operator>=(const BigInt &out, const Number_T number) noexcept {
        return (((out._index != 0U) || (out._storage[0U] >= number)));
    }

    inline friend bool operator>=(const Number_T number, const BigInt &out) noexcept {
        return (out <= number);
    }

    inline friend bool operator==(const BigInt &out, const Number_T number) noexcept {
        return (((out._index == 0U) && (out._storage[0U] == number)));
    }

    inline friend bool operator==(const Number_T number, const BigInt &out) noexcept {
        return (out == number);
    }

    inline friend bool operator!=(const BigInt &out, const Number_T number) noexcept {
        return (((out._index != 0U) || (out._storage[0U] != number)));
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
        if (number != Number_T{0}) {
            while (index <= MaxIndex()) {
                const Number_T tmp = _storage[index];
                _storage[index] += number;

                if (_storage[index] > tmp) {
                    break;
                }

                // Overflow.
                number = Number_T{1};
                ++index;
            }

            _index += SizeT32((index > _index) & (index <= MaxIndex()));
        }
    }

    inline void Subtract(Number_T number, SizeT32 index = 0U) noexcept {
        if (number != Number_T{0}) {
            while (index <= MaxIndex()) {
                const Number_T tmp = _storage[index];
                _storage[index] -= number;

                if (_storage[index] < tmp) {
                    break;
                }

                // Overflow.
                number = Number_T{1};
                ++index;
            }

            _index -= SizeT32((index > 0U) && (_storage[_index] == Number_T{0}));
        }
    }
    ////////////////////////////////////////////////////
    inline void Multiply(Number_T multiplier) noexcept {
        SizeT32 index = _index;
        ++index;

        do {
            --index;
            Add(DoubleSize<Number_T, TypeWidth()>::Multiply(_storage[index], multiplier), (index + 1U));
        } while (index != 0U);
    }
    ////////////////////////////////////////////////////
    inline Number_T Divide(const Number_T divisor) noexcept {
        constexpr bool is_size_64b = (TypeWidth() == 64U);
        SizeT32        index       = _index;
        Number_T       remainder   = (_storage[_index] % divisor);
        _storage[_index] /= divisor;

        const SizeT32 initial_shift = [=]() noexcept -> SizeT32 {
            if (is_size_64b) {
                return ((TypeWidth() - 1U) - Platform::FindLastBit(divisor));
            } else {
                return 0U;
            }
        }();

        while (index != 0U) {
            --index;
            DoubleSize<Number_T, TypeWidth()>::Divide(remainder, _storage[index], divisor, initial_shift);
        }

        _index -= SizeT32((_index > 0U) && (_storage[_index] == Number_T{0}));

        return remainder;
    }
    ////////////////////////////////////////////////////
    inline void ShiftRight(SizeT32 offset) noexcept {
        // while (offset >= TypeWidth()) {
        //     SizeT32 index = 0U;

        //     while (index < _index) {
        //         _storage[index] = _storage[index + 1U];
        //         ++index;
        //     }

        //     _storage[_index] = Number_T{0};
        //     _index -= SizeT32(_index != 0U);

        //     offset -= TypeWidth();
        // }

        if (offset >= TypeWidth()) {
            SizeT32 move = (offset / TypeWidth());
            offset -= (move * TypeWidth());

            if (move > _index) {
                Clear();
                return;
            }

            SizeT32 index = 0U;
            SizeT32 next  = (index + move);

            do {
                _storage[index] = _storage[next];
                ++index;
                ++next;
            } while (next <= _index);

            do {
                _storage[_index] = Number_T{0};
                --_index;
                --move;
            } while (move != 0U);
        }

        if (offset != 0U) {
            SizeT32       index      = 0U;
            const SizeT32 shift_size = (TypeWidth() - offset);

            _storage[0U] >>= offset;

            while (index < _index) {
                _storage[index] |= (_storage[index + 1U] << shift_size);
                ++index;
                _storage[index] >>= offset;
            }

            _index -= SizeT32((_index != 0U) && (_storage[_index] == Number_T{0}));
        }
    }

    inline void ShiftLeft(SizeT32 offset) noexcept {
        if (offset >= TypeWidth()) {
            SizeT32 move = (offset / TypeWidth());
            offset -= (move * TypeWidth());

            SizeT32 index = _index;
            index += move;

            if (index > MaxIndex()) {
                SizeT32 diff = (index - MaxIndex());

                if (diff <= _index) {
                    _index -= diff;
                    index = MaxIndex();
                } else {
                    Clear();
                    return;
                }
            }

            _storage[_index + move] = _storage[_index];

            while (_index != 0U) {
                --_index;
                _storage[_index + move] = _storage[_index];
            }

            do {
                --move;
                _storage[move] = Number_T{0};
            } while (move != 0U);

            while (_storage[index] == Number_T{0}) {
                --index;
            }

            _index = index;
        }

        // SizeT32 index = _index;

        // while (offset >= TypeWidth()) {
        //     _index += SizeT32((_storage[index] != 0U) && (_index < MaxIndex()));
        //     _storage[_index] = _storage[index];

        //     while (index != 0U) {
        //         _storage[index] = _storage[index - 1U];
        //         --index;
        //     }

        //     _storage[0U] = Number_T{0};
        //     index        = _index;
        //     offset -= TypeWidth();
        // }

        if (offset != 0U) {
            SizeT32       index      = _index;
            const SizeT32 shift_size = (TypeWidth() - offset);

            const Number_T carry = (_storage[index] >> shift_size);
            _storage[index] <<= offset;

            _index += SizeT32((carry != Number_T{0}) && (_index < MaxIndex()));
            _storage[_index] |= carry;

            while (index != 0U) {
                _storage[index] |= (_storage[index - 1U] >> shift_size);
                --index;
                _storage[index] <<= offset;
            }
        }
    }
    ////////////////////////////////////////////////////
    inline SizeT32 FindFirstBit() const noexcept {
        SizeT32 index = 0U;

        while ((_storage[index] == Number_T{0}) && (index <= _index)) {
            ++index;
        }

        return (Platform::FindFirstBit(_storage[_index]) + (index * TypeWidth()));
    }

    inline SizeT32 FindLastBit() const noexcept {
        return (Platform::FindLastBit(_storage[_index]) + SizeT32(_index * TypeWidth()));
    }
    ////////////////////////////////////////////////////
    inline Number_T Number() const noexcept {
        return _storage[0U];
    }

    inline void Clear() noexcept {
        _storage[0U] = Number_T{0};

        while (_index != 0U) {
            _storage[_index] = Number_T{0};
            --_index;
        }
    }

    inline SizeT32 Index() const noexcept {
        return _index;
    }

    inline static constexpr SizeT32 MaxIndex() noexcept {
        return (((TypeWidth() * (TotalBits() / TypeWidth())) == TotalBits()) ? ((TotalBits() / TypeWidth()) - 1U)
                                                                             : (TotalBits() / TypeWidth()));
    }

    inline static constexpr SizeT32 TypeWidth() noexcept {
        return (SizeOfType() * 8U);
    }

    inline static constexpr SizeT32 TotalBits() noexcept {
        return Width_T;
    }

    inline static constexpr SizeT32 SizeOfType() noexcept {
        return sizeof(Number_T);
    }

    inline bool IsBig() const noexcept {
        return (_index != 0U);
    }

    inline bool NotZero() const noexcept {
        return (*this != Number_T{0});
    }

    inline bool IsZero() const noexcept {
        return (*this == Number_T{0});
    }

    inline void SetIndex(SizeT32 index) noexcept {
        _index = index;
    }

    inline Number_T *Storage() noexcept {
        return _storage;
    }

    inline const Number_T *Storage() const noexcept {
        return _storage;
    }

  private:
    Number_T _storage[MaxIndex() + Number_T{1}]{0};
    SizeT32  _index{0};

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
                _storage[0U] |= number;
                break;
            }

            case BigIntOperation::And: {
                _storage[0U] &= number;
                _index = 0U;
                break;
            }

            default: {
                _storage[0U] = number;
                _index       = 0U;
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
                _storage[0U] |= Number_T(number);
                break;
            }

            case BigIntOperation::And: {
                _storage[0U] &= Number_T(number);
                _index = 0U;
                break;
            }

            default: {
                _storage[0U] = Number_T(number);
                _index       = 0U;
            }
        }

        if (is_bigger_size) {
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
                        _storage[index] |= Number_T(number);

                        if (index > _index) {
                            _index = index;
                        }

                        break;
                    }

                    case BigIntOperation::And: {
                        _storage[index] &= Number_T(number);

                        if (_storage[index] != Number_T(0)) {
                            _index = index;
                        }

                        break;
                    }

                    default: {
                        _storage[index] = Number_T(number);
                        ++_index;
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
        dividend16 <<= _shift;
        dividend16 |= dividend_low;
        dividend_high = Number_T(dividend16 % divisor);
        dividend16 /= divisor;
        dividend_low = Number_T(dividend16);
    }

    inline static Number_T Multiply(Number_T &number, const Number_T multiplier) noexcept {
        SizeT16 number16{number};
        number16 *= multiplier;
        number = Number_T(number16);

        return Number_T(number16 >> _shift);
    }

  private:
    static constexpr SizeT32 _shift = 8U;
};
////////////////////////////////////////////////////
template <typename Number_T>
struct DoubleSize<Number_T, 16U> {
    inline static void Divide(Number_T &dividend_high, Number_T &dividend_low, const Number_T divisor,
                              SizeT32) noexcept {
        SizeT32 dividend32{dividend_high};
        dividend32 <<= _shift;
        dividend32 |= dividend_low;
        dividend_high = Number_T(dividend32 % divisor);
        dividend32 /= divisor;
        dividend_low = Number_T(dividend32);
    }

    inline static Number_T Multiply(Number_T &number, const Number_T multiplier) noexcept {
        SizeT32 number32{number};
        number32 *= multiplier;
        number = Number_T(number32);

        return Number_T(number32 >> _shift);
    }

  private:
    static constexpr SizeT32 _shift = 16U;
};
////////////////////////////////////////////////////
template <typename Number_T>
struct DoubleSize<Number_T, 32U> {
    inline static void Divide(Number_T &dividend_high, Number_T &dividend_low, const Number_T divisor,
                              SizeT32) noexcept {
        SizeT64 dividend64{dividend_high};
        dividend64 <<= _shift;
        dividend64 |= dividend_low;
        dividend_high = Number_T(dividend64 % divisor);
        dividend64 /= divisor;
        dividend_low = Number_T(dividend64);
    }

    inline static Number_T Multiply(Number_T &number, const Number_T multiplier) noexcept {
        SizeT64 number64{number};
        number64 *= multiplier;
        number = Number_T(number64);

        return Number_T(number64 >> _shift);
    }

  private:
    static constexpr SizeT32 _shift = 32U;
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
        const Number_T divisor_low  = (divisor_shifted >> _shift);
        const Number_T divisor_high = (divisor_shifted & _mask);
        // -----------------------
        dividend_high <<= initial_shift;
        // -----------------------
        Number_T quotient = (dividend_high / divisor_low);
        dividend_high %= divisor_low;
        Number_T reminder = (quotient * divisor_high);

        dividend_high <<= _shift;

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
        quotient <<= _shift;
        dividend_low += quotient;
        // -----------------------
        quotient = (dividend_high / divisor_low);
        dividend_high %= divisor_low;
        reminder = (quotient * divisor_high);

        dividend_high <<= _shift;

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
        const Number_T original_dividend_high = dividend_high;
        dividend_high += carry;
        // -----------------------
        if (original_dividend_high > dividend_high) {
            // Overflow
            constexpr Number_T overflow_dividend = (Number_T{1} << (_width - 1U));

            dividend_high += ((overflow_dividend % (divisor >> 1U)) << 1U);
            ++dividend_low;
        }

        if (dividend_high >= divisor) {
            dividend_high -= divisor;
            ++dividend_low;
        }
    }

    inline static Number_T Multiply(Number_T &number, Number_T multiplier) noexcept {
        const Number_T number_low     = (number & _mask);
        Number_T       number_high    = number;
        Number_T       multiplier_low = (multiplier & _mask);

        number = (number_low * multiplier_low);

        number_high >>= _shift;
        multiplier_low *= number_high;
        multiplier_low += (number >> _shift);
        number &= _mask;

        multiplier >>= _shift;
        number_high *= multiplier;
        number_high += (multiplier_low >> _shift);

        multiplier_low &= _mask;
        multiplier_low += (number_low * multiplier);

        number |= (multiplier_low << _shift);
        multiplier_low >>= _shift;
        number_high += multiplier_low;

        return number_high;
    }

  private:
    static constexpr SizeT32  _width = (sizeof(Number_T) * 8U);
    static constexpr SizeT32  _shift = (_width / 2U);
    static constexpr Number_T _mask  = (~(Number_T{0}) >> _shift);
};

} // namespace Qentem

#endif
