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

#ifndef _QENTEM_BIG_INT_H
#define _QENTEM_BIG_INT_H

namespace Qentem {

// _Width_T is the needed number of bits:
// 2 64bit int = 128 bits
// 6 64bit int = 384 bits
// 16 64bit int = 1024 bits
// (number of variables) * sizeof(variable) * 8 = _Width_T

template <typename _Number_T, SizeT32>
struct DoubleSize {};

template <typename _Number_T, SizeT32 _Width_T>
struct BigInt {
    using _NumberType = _Number_T;

    enum class BigIntOperation : SizeT8 {
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

    template <typename _N_Number_T>
    inline BigInt(const _N_Number_T number) noexcept {
        multiOperation<BigIntOperation::Set>(number);
    }

    template <typename _N_Number_T>
    inline BigInt &operator=(const _N_Number_T number) noexcept {
        while (_index_id != _Number_T{0}) {
            _big_int[_index_id] = _Number_T{0};
            --_index_id;
        }

        multiOperation<BigIntOperation::Set>(number);

        return *this;
    }

    template <typename _N_Number_T>
    inline explicit operator _N_Number_T() const noexcept {
        constexpr SizeT32 n_size          = (sizeof(_N_Number_T) * 8U);
        constexpr bool    is_same_size    = (n_size == BitSize());
        constexpr bool    is_smaller_size = (n_size < BitSize());

        if (is_same_size) {
            return _big_int[0];
        } else if (is_smaller_size) {
            return _N_Number_T(_big_int[0]);
        } else {
            _N_Number_T num   = 0;
            SizeT32     count = (n_size / BitSize());

            while ((--count != SizeT32{0}) && (count >= _index_id)) {
                num |= _big_int[1];
                num <<= BitSize();
            }

            num |= _big_int[0];

            return num;
        }
    }

    inline friend bool operator<(const BigInt &out, const _Number_T number) noexcept {
        return (((out._index_id == _Number_T{0}) && (out._big_int[0U] < number)));
    }

    inline friend bool operator<(const _Number_T number, const BigInt &out) noexcept {
        return (out > number);
    }

    inline friend bool operator<=(const BigInt &out, const _Number_T number) noexcept {
        return (((out._index_id == _Number_T{0}) && (out._big_int[0U] <= number)));
    }

    inline friend bool operator<=(const _Number_T number, const BigInt &out) noexcept {
        return (out >= number);
    }

    inline friend bool operator>(const BigInt &out, const _Number_T number) noexcept {
        return (((out._index_id != _Number_T{0}) || (out._big_int[0U] > number)));
    }

    inline friend bool operator>(const _Number_T number, const BigInt &out) noexcept {
        return (out < number);
    }

    inline friend bool operator>=(const BigInt &out, const _Number_T number) noexcept {
        return (((out._index_id != _Number_T{0}) || (out._big_int[0U] >= number)));
    }

    inline friend bool operator>=(const _Number_T number, const BigInt &out) noexcept {
        return (out <= number);
    }

    inline friend bool operator==(const BigInt &out, const _Number_T number) noexcept {
        return (((out._index_id == _Number_T{0}) && (out._big_int[0U] == number)));
    }

    inline friend bool operator==(const _Number_T number, const BigInt &out) noexcept {
        return (out == number);
    }

    inline friend bool operator!=(const BigInt &out, const _Number_T number) noexcept {
        return (((out._index_id != _Number_T{0}) || (out._big_int[0U] != number)));
    }

    inline friend bool operator!=(const _Number_T number, const BigInt &out) noexcept {
        return (out != number);
    }

    inline void operator<<=(const SizeT32 number) noexcept {
        ShiftLeft(number);
    }

    inline void operator>>=(const SizeT32 number) noexcept {
        ShiftRight(number);
    }

    template <typename _N_Number_T>
    inline void operator|=(const _N_Number_T number) noexcept {
        multiOperation<BigIntOperation::Or>(number);
    }

    template <typename _N_Number_T>
    inline void operator&=(const _N_Number_T number) noexcept {
        multiOperation<BigIntOperation::And>(number);
    }

    template <typename _N_Number_T>
    inline void operator+=(const _N_Number_T number) noexcept {
        multiOperation<BigIntOperation::Add>(number);
    }

    template <typename _N_Number_T>
    inline void operator-=(const _N_Number_T number) noexcept {
        multiOperation<BigIntOperation::Subtract>(number);
    }

    inline void operator*=(const _Number_T number) noexcept {
        Multiply(number);
    }

    inline void operator/=(const _Number_T number) noexcept {
        Divide(number);
    }
    ////////////////////////////////////////////////////
    void Add(_Number_T number, _Number_T index = 0) noexcept {
        if (number != _Number_T{0}) {
            while (index <= MaxIndex()) {
                const _Number_T tmp = _big_int[index];
                _big_int[index] += number;

                if (_big_int[index] > tmp) {
                    break;
                }

                // Overflow.
                number = _Number_T{1};
                ++index;
            }

            _index_id += ((index > _index_id) && (index <= MaxIndex()));
        }
    }

    inline void Subtract(_Number_T number, _Number_T index = 0) noexcept {
        if (number != _Number_T{0}) {
            while (index <= MaxIndex()) {
                const _Number_T tmp = _big_int[index];
                _big_int[index] -= number;

                if (_big_int[index] < tmp) {
                    break;
                }

                // Overflow.
                number = _Number_T{1};
                ++index;
            }

            _index_id -= ((index > _Number_T{0}) && (_big_int[_index_id] == _Number_T{0}));
        }
    }
    ////////////////////////////////////////////////////
    inline void Multiply(_Number_T multiplier) noexcept {
        _Number_T index = (_index_id + _Number_T{1});

        do {
            --index;
            Add(DoubleSize<_Number_T, BitSize()>::Multiply(_big_int[index], multiplier), (index + _Number_T{1}));
        } while (index != _Number_T{0});
    }
    ////////////////////////////////////////////////////
    inline _Number_T Divide(const _Number_T divisor) noexcept {
        constexpr bool is_size_64b = (BitSize() == 64U);
        _Number_T      index       = _index_id;
        _Number_T      remainder   = (_big_int[_index_id] % divisor);
        _big_int[_index_id] /= divisor;

        const SizeT32 initial_shift = [=]() noexcept -> SizeT32 {
            if (is_size_64b) {
                return ((BitSize() - 1U) - Platform::FindLastBit(divisor));
            } else {
                return 0U;
            }
        }();

        while (index != _Number_T{0}) {
            --index;
            DoubleSize<_Number_T, BitSize()>::Divide(remainder, _big_int[index], divisor, initial_shift);
        }

        _index_id -= ((_index_id > _Number_T{0}) && (_big_int[_index_id] == _Number_T{0}));

        return remainder;
    }
    ////////////////////////////////////////////////////
    inline void ShiftRight(SizeT32 offset) noexcept {
        _Number_T index = 0U;

        while (offset >= BitSize()) {
            while (index < _index_id) {
                _big_int[index] = _big_int[index + _Number_T{1}];
                ++index;
            }

            _big_int[_index_id] = _Number_T{0};
            _index_id -= (_index_id != _Number_T{0});

            index = _Number_T{0};
            offset -= BitSize();
        }

        if (offset != _Number_T{0}) {
            const SizeT32 shift_size = (BitSize() - offset);

            _big_int[_Number_T{0}] >>= offset;

            while (index < _index_id) {
                _big_int[index] |= (_big_int[index + _Number_T{1}] << shift_size);
                ++index;
                _big_int[index] >>= offset;
            }

            _index_id -= ((_index_id != _Number_T{0}) && (_big_int[_index_id] == _Number_T{0}));
        }
    }

    inline void ShiftLeft(SizeT32 offset) noexcept {
        _Number_T index = _index_id;

        while (offset >= BitSize()) {
            _index_id += ((_big_int[index] != _Number_T{0}) && (_index_id < MaxIndex()));
            _big_int[_index_id] = _big_int[index];

            while (index != _Number_T{0}) {
                _big_int[index] = _big_int[index - _Number_T{1}];
                --index;
            }

            _big_int[0U] = _Number_T{0};
            index        = _index_id;
            offset -= BitSize();
        }

        if (offset != _Number_T{0}) {
            const SizeT32 shift_size = (BitSize() - offset);

            const _Number_T carry = (_big_int[index] >> shift_size);
            _big_int[index] <<= offset;

            _index_id += ((carry != _Number_T{0}) && (_index_id < MaxIndex()));
            _big_int[_index_id] |= carry;

            while (index != _Number_T{0}) {
                _big_int[index] |= (_big_int[index - _Number_T{1}] >> shift_size);
                --index;
                _big_int[index] <<= offset;
            }
        }
    }
    ////////////////////////////////////////////////////
    inline SizeT32 FindFirstBit() const noexcept {
        _Number_T index = _Number_T{0};

        while ((_big_int[index] == _Number_T{0}) && (index <= _index_id)) {
            ++index;
        }

        return (Platform::FindFirstBit(_big_int[_index_id]) + (index * BitSize()));
    }

    inline SizeT32 FindLastBit() const noexcept {
        return (Platform::FindLastBit(_big_int[_index_id]) + SizeT32(_index_id * BitSize()));
    }
    ////////////////////////////////////////////////////
    inline _Number_T Number() const noexcept {
        return _big_int[0U];
    }

    inline _Number_T const *Bucket(const SizeT32 index) const noexcept {
        if (index <= _index_id) {
            return (_big_int + index);
        }

        return nullptr;
    }

    inline _Number_T *Bucket(const SizeT32 index) noexcept {
        if (index <= _index_id) {
            return (_big_int + index);
        }

        return nullptr;
    }

    inline _Number_T Index() const noexcept {
        return _index_id;
    }

    inline static constexpr SizeT32 MaxIndex() noexcept {
        return (((BitSize() * (TotalBits() / BitSize())) == TotalBits()) ? (TotalBits() / BitSize())
                                                                         : ((TotalBits() / BitSize()) + SizeT32{1}));
    }

    inline static constexpr SizeT32 BitSize() noexcept {
        return (SizeOfType() * 8U);
    }

    inline static constexpr SizeT32 TotalBits() noexcept {
        return _Width_T;
    }

    inline static constexpr SizeT32 SizeOfType() noexcept {
        return sizeof(_Number_T);
    }

    inline bool IsBig() const noexcept {
        return (_index_id != _Number_T{0});
    }

    inline bool NotZero() const noexcept {
        return (*this != _Number_T{0});
    }

    inline bool IsZero() const noexcept {
        return (*this == _Number_T{0});
    }

    inline void SetIndex(_Number_T id) noexcept {
        _index_id = id;
    }

    inline _Number_T *Storage() noexcept {
        return _big_int;
    }

  private:
    _Number_T _big_int[MaxIndex() + _Number_T{1}]{0};
    _Number_T _index_id{0};

    template <BigIntOperation Operation>
    inline void multiOperation(_Number_T number) noexcept {
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
                _big_int[0U] |= number;
                break;
            }

            case BigIntOperation::And: {
                _big_int[0U] &= number;
                break;
            }

            default: {
                _big_int[0U] = number;
            }
        }
    }

    template <BigIntOperation Operation, typename _N_Number_T>
    inline void multiOperation(_N_Number_T number) noexcept {
        constexpr SizeT32 n_size         = ((sizeof(_N_Number_T) * 8U) / BitSize());
        constexpr bool    is_bigger_size = (n_size > 1U);

        switch (Operation) {
            case BigIntOperation::Add: {
                Add(_Number_T(number));
                break;
            }

            case BigIntOperation::Subtract: {
                Subtract(_Number_T(number));
                break;
            }

            case BigIntOperation::Or: {
                _big_int[0U] |= _Number_T(number);
                break;
            }

            case BigIntOperation::And: {
                _big_int[0U] &= _Number_T(number);
                break;
            }

            default: {
                _big_int[0U] = _Number_T(number);
            }
        }

        if (is_bigger_size) {
            number >>= BitSize();

            while (number != _N_Number_T{0}) {
                ++_index_id;

                switch (Operation) {
                    case BigIntOperation::Add: {
                        Add(_Number_T(number), _index_id);
                        break;
                    }

                    case BigIntOperation::Subtract: {
                        Subtract(_Number_T(number), _index_id);
                        break;
                    }

                    case BigIntOperation::Or: {
                        _big_int[_index_id] |= _Number_T(number);
                        break;
                    }

                    case BigIntOperation::And: {
                        _big_int[_index_id] &= _Number_T(number);
                        break;
                    }

                    default: {
                        _big_int[_index_id] = _Number_T(number);
                    }
                }

                number >>= BitSize();
            }
        }
    }
};
////////////////////////////////////////////////////
template <typename _Number_T>
struct DoubleSize<_Number_T, 8U> {
    static constexpr SizeT32 shift = 8U;

    inline static void Divide(_Number_T &dividend_high, _Number_T &dividend_low, const _Number_T divisor,
                              const SizeT32 initial_shift) noexcept {
        (void)initial_shift;

        SizeT16 dividend16 = dividend_high;
        dividend16 <<= shift;
        dividend16 |= dividend_low;
        dividend_high = _Number_T(dividend16 % divisor);
        dividend16 /= divisor;
        dividend_low = _Number_T(dividend16);
    }

    inline static _Number_T Multiply(_Number_T &number, const _Number_T multiplier) noexcept {
        SizeT16 number16 = number;
        number16 *= multiplier;
        number = _Number_T(number16);

        return _Number_T(number16 >> shift);
    }
};
////////////////////////////////////////////////////
template <typename _Number_T>
struct DoubleSize<_Number_T, 16U> {
    static constexpr SizeT32 shift = 16U;

    inline static void Divide(_Number_T &dividend_high, _Number_T &dividend_low, const _Number_T divisor,
                              const SizeT32 initial_shift) noexcept {
        (void)initial_shift;

        SizeT32 dividend32 = dividend_high;
        dividend32 <<= shift;
        dividend32 |= dividend_low;
        dividend_high = _Number_T(dividend32 % divisor);
        dividend32 /= divisor;
        dividend_low = _Number_T(dividend32);
    }

    inline static _Number_T Multiply(_Number_T &number, const _Number_T multiplier) noexcept {
        SizeT32 number32 = number;
        number32 *= multiplier;
        number = _Number_T(number32);

        return _Number_T(number32 >> shift);
    }
};
////////////////////////////////////////////////////
template <typename _Number_T>
struct DoubleSize<_Number_T, 32U> {
    static constexpr SizeT32 shift = 32U;

    inline static void Divide(_Number_T &dividend_high, _Number_T &dividend_low, const _Number_T divisor,
                              const SizeT32 initial_shift) noexcept {
        (void)initial_shift;

        SizeT64 dividend64 = dividend_high;
        dividend64 <<= shift;
        dividend64 |= dividend_low;
        dividend_high = _Number_T(dividend64 % divisor);
        dividend64 /= divisor;
        dividend_low = _Number_T(dividend64);
    }

    inline static _Number_T Multiply(_Number_T &number, const _Number_T multiplier) noexcept {
        SizeT64 number64 = number;
        number64 *= multiplier;
        number = _Number_T(number64);

        return _Number_T(number64 >> shift);
    }
};
////////////////////////////////////////////////////
template <typename _Number_T>
struct DoubleSize<_Number_T, 64U> {
    inline static void Divide(_Number_T &dividend_high, _Number_T &dividend_low, const _Number_T divisor,
                              const SizeT32 initial_shift) noexcept {
        const _Number_T carry = (dividend_low % divisor);
        dividend_low /= divisor;
        // -----------------------
        const _Number_T divisor_shifted = (divisor << initial_shift);
        // -----------------------
        const _Number_T divisor_low  = (divisor_shifted >> shift);
        const _Number_T divisor_high = (divisor_shifted & mask);
        // -----------------------
        dividend_high <<= initial_shift;
        // -----------------------
        _Number_T quotient = (dividend_high / divisor_low);
        dividend_high %= divisor_low;
        _Number_T reminder = (quotient * divisor_high);

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
        const _Number_T original_dividend_high = dividend_high;
        dividend_high += carry;
        // -----------------------
        if (original_dividend_high > dividend_high) {
            // Overflow
            constexpr _Number_T overflow_dividend = (_Number_T{1} << (width - 1U));

            dividend_high += ((overflow_dividend % (divisor >> 1U)) << 1U);
            ++dividend_low;
        }

        if (dividend_high >= divisor) {
            dividend_high -= divisor;
            ++dividend_low;
        }
    }

    inline static _Number_T Multiply(_Number_T &number, _Number_T multiplier) noexcept {
        const _Number_T number_low     = (number & mask);
        _Number_T       number_high    = number;
        _Number_T       multiplier_low = (multiplier & mask);

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
    static constexpr SizeT32   width = (sizeof(_Number_T) * 8U);
    static constexpr SizeT32   shift = (width / 2U);
    static constexpr _Number_T mask  = (~(_Number_T{0}) >> shift);
};

} // namespace Qentem

#endif
