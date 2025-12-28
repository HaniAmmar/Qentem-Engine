/**
 * @file BigInt.hpp
 * @brief High-performance arbitrary-precision integer arithmetic for Qentem Engine.
 *
 * This header defines the BigInt class template, providing efficient, portable support
 * for unsigned integers of virtually unlimited size. Designed for speed and reliability,
 * BigInt leverages advanced double-width arithmetic algorithms for multi-limb operations—
 * outperforming even compiler-provided 128-bit types on most platforms.
 *
 * Typical Use Cases:
 * - Accurate floating-point to string conversion
 * - Cryptography (RSA, Diffie-Hellman, ElGamal, elliptic curve field arithmetic)
 * - Modular exponentiation and modular inversion
 * - Barrett and Montgomery reduction for fast modular arithmetic
 * - Primality testing (Miller-Rabin, Fermat, etc.)
 * - Arbitrary-precision number theory and mathematics
 * - Scientific computation (very large integers or extended-precision math)
 * - Implementing custom numeric types (rational, big decimal, etc.)
 * - Parsing and encoding large binary fields (protocols, file formats, blockchain)
 * - Checksum and hashing algorithms with bignum state
 * - Simulation and symbolic computation
 *
 * Key Features:
 * - Optimized for modern C++: No external dependencies, header-only, and portable.
 * - Limb-based design: Uses any unsigned integer type for limbs (e.g., uint16_t, uint32_t, uint64_t).
 * - Superior performance: Includes hand-crafted double-limb multiplication and division,
 *   outpacing `__int128`-based approaches on major compilers (GCC, Clang, MSVC).
 * - Production-grade reliability: Clear separation of algorithms for add, subtract,
 *   multiply, divide, and bitwise operations—fully documented and tested.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 *
 * @note This implementation is tuned for unsigned integer limbs. Signed bignum support,
 *       if needed, can be layered on top.
 * @note For best performance, prefer 32- or 64-bit limbs on modern hardware.
 *
 * @usage
 *     #include "BigInt.hpp"
 *     using Qentem::BigInt;
 *
 *     // 128-bit unsigned integer using 64-bit limbs:
 *     BigInt<unsigned long long, 128U> value = 0xFFFFFFFFFFFFFFFF;
 *     value <<= 64U;
 *     value |= 0xFFFFFFFFFFFFFFFF;
 */

#ifndef QENTEM_BIG_INT_H
#define QENTEM_BIG_INT_H

#include "Platform.hpp"

namespace Qentem {
// Width_T is the total number of bits to represent.
// Example: For 128 bits using 64-bit limbs, Width_T = 128.
// 2 limbs (uint64_t) × 64 = 128 bits
// 6 limbs (uint64_t) × 64 = 384 bits
// 16 limbs (uint64_t) × 64 = 1024 bits
// In general: (number of limbs) * sizeof(Number_T) * 8 = Width_T

/**
 * @brief Helper struct for double-width operations (multiply/divide).
 * @tparam Number_T  Limb type (unsigned integer).
 * @tparam (empty)   Placeholder for partial specialization.
 */
template <typename Number_T, SizeT32>
struct DoubleWidthArithmetic {};

/**
 * @brief Fixed-width arbitrary-precision unsigned integer.
 * @tparam Number_T Limb type (must be unsigned).
 * @tparam Width_T  Total bit width of the BigInt.
 *
 * Example:
 *     // 256-bit unsigned integer using 64-bit limbs:
 *     BigInt<uint64_t, 256U> val;
 */
template <typename Number_T, SizeT32 Width_T>
struct BigInt {
    using NumberType = Number_T;

    /**
     * @brief Internal operations used by BigInt for template-based arithmetic.
     */
    enum struct BigIntOperation : SizeT8 {
        Set = 0, ///< Assignment (=)
        Or,      ///< Bitwise OR (|)
        And,     ///< Bitwise AND (&)
        Add,     ///< Addition (+)
        Subtract ///< Subtraction (-)
    };

    /**
     * @brief Default constructor. Initializes to zero.
     */
    BigInt() noexcept = default;

    /**
     * @brief Destructor.
     */
    ~BigInt() noexcept = default;

    /**
     * @brief Move constructor.
     * @param src Source BigInt to move from. The source is cleared after move.
     */
    QENTEM_INLINE BigInt(BigInt &&src) noexcept : index_{src.index_} {
        SizeT32 index = 0;
        // Move all limbs from src to this BigInt
        while (index <= index_) {
            storage_[index] = src.storage_[index];
            ++index;
        }

        src.Clear();
    }

    /**
     * @brief Copy constructor.
     * @param src Source BigInt to copy from.
     */
    QENTEM_INLINE BigInt(const BigInt &src) noexcept : index_{src.index_} {
        SizeT32 index = 0;
        // Copy all limbs from src to this BigInt
        while (index <= index_) {
            storage_[index] = src.storage_[index];
            ++index;
        }
    }

    /**
     * @brief Construct BigInt from a built-in integer type.
     * @tparam N_Number_T Any integer type (usually unsigned).
     * @param number Value to initialize with.
     */
    template <typename N_Number_T>
    QENTEM_INLINE BigInt(const N_Number_T number) noexcept {
        doOperation<BigIntOperation::Set>(number);
    }

    /**
     * @brief Assign BigInt from a built-in integer type.
     * @tparam N_Number_T Any integer type (usually unsigned).
     * @param number Value to assign from.
     * @return Reference to this BigInt.
     *
     * Unused higher limbs are cleared.
     */
    template <typename N_Number_T>
    QENTEM_INLINE BigInt &operator=(const N_Number_T number) noexcept {
        SizeT32 index = index_;
        doOperation<BigIntOperation::Set>(number);

        // Zero out unused higher limbs, if any
        while (index > index_) {
            storage_[index] = 0;
            --index;
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param src Source BigInt to move from. The source is cleared after assignment.
     * @return Reference to this BigInt.
     */
    QENTEM_INLINE BigInt &operator=(BigInt &&src) noexcept {
        if (this != &src) {
            copy(src);
            src.Clear();
        }

        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param src Source BigInt to copy from.
     * @return Reference to this BigInt.
     */
    QENTEM_INLINE BigInt &operator=(const BigInt &src) noexcept {
        if (this != &src) {
            copy(src);
        }

        return *this;
    }

    /**
     * @brief Converts this BigInt to a built-in integer type.
     * @tparam N_Number_T Target integer type (e.g., uint64_t, unsigned int).
     * @return The value as N_Number_T. If the BigInt does not fit into the target type,
     *         only the least significant bits are preserved (higher bits are truncated).
     *
     * Example:
     *     BigInt<uint64_t, 256> big = ...;
     *     uint32_t val = static_cast<uint32_t>(big); // Only the lowest 32 bits are used.
     */
    template <typename N_Number_T>
    explicit operator N_Number_T() const noexcept {
        // Number of bits in the target type
        constexpr SizeT32 n_width = (sizeof(N_Number_T) * 8U);

        // True if target type fits in a single limb or is equal width
        constexpr bool is_same_size    = (n_width == BitWidth());
        constexpr bool is_smaller_size = (n_width < BitWidth());

        // Fast path: fits in a single limb, just return storage_[0] truncated
        if constexpr (is_smaller_size || is_same_size) {
            return N_Number_T(storage_[0]);
        } else {
            // Otherwise, need to assemble from multiple limbs
            constexpr SizeT32 max_index = ((n_width / BitWidth()) - 1U);
            // Limit to the number of non-zero limbs present in this BigInt
            SizeT32    index = ((max_index <= index_) ? max_index : index_);
            N_Number_T num{0};

            // Loop through limbs from high to low, combining bits
            while (index != 0) {
                num |= storage_[index];
                num <<= BitWidth();
                --index;
            }

            // Add in the lowest limb
            num |= storage_[0];
            return num;
        }
    }

    /**
     * @brief Compares BigInt with a built-in integer for less-than.
     * @param out   The BigInt instance (left operand).
     * @param number The built-in integer (right operand).
     * @return True if BigInt is less than number.
     *
     * Only true if the BigInt fits in a single limb.
     */
    QENTEM_INLINE friend bool operator<(const BigInt &out, const Number_T number) noexcept {
        // If BigInt has only one limb, compare directly.
        return ((out.index_ == 0) && (out.storage_[0] < number));
    }

    /**
     * @brief Compares a built-in integer with a BigInt for less-than.
     * @param number The built-in integer (left operand).
     * @param out    The BigInt instance (right operand).
     * @return True if number is less than BigInt.
     */
    QENTEM_INLINE friend bool operator<(const Number_T number, const BigInt &out) noexcept {
        // Reuse the greater-than operator
        return (out > number);
    }

    /**
     * @brief Compares BigInt with a built-in integer for less-than-or-equal.
     * @param out    The BigInt instance (left operand).
     * @param number The built-in integer (right operand).
     * @return True if BigInt is less than or equal to number.
     */
    QENTEM_INLINE friend bool operator<=(const BigInt &out, const Number_T number) noexcept {
        // If BigInt has only one limb, compare directly.
        return ((out.index_ == 0) && (out.storage_[0] <= number));
    }

    /**
     * @brief Compares a built-in integer with a BigInt for less-than-or-equal.
     * @param number The built-in integer (left operand).
     * @param out    The BigInt instance (right operand).
     * @return True if number is less than or equal to BigInt.
     */
    QENTEM_INLINE friend bool operator<=(const Number_T number, const BigInt &out) noexcept {
        // Reuse the greater-than-or-equal operator
        return (out >= number);
    }

    /**
     * @brief Compares BigInt with a built-in integer for greater-than.
     * @param out    The BigInt instance (left operand).
     * @param number The built-in integer (right operand).
     * @return True if BigInt is greater than number.
     *
     * True if BigInt has more than one limb or the value is greater than number.
     */
    QENTEM_INLINE friend bool operator>(const BigInt &out, const Number_T number) noexcept {
        // If any higher limb is used, BigInt is greater.
        return ((out.index_ != 0) || (out.storage_[0] > number));
    }

    /**
     * @brief Compares a built-in integer with a BigInt for greater-than.
     * @param number The built-in integer (left operand).
     * @param out    The BigInt instance (right operand).
     * @return True if number is greater than BigInt.
     */
    QENTEM_INLINE friend bool operator>(const Number_T number, const BigInt &out) noexcept {
        // Reuse less-than operator
        return (out < number);
    }

    /**
     * @brief Compares BigInt with a built-in integer for greater-than-or-equal.
     * @param out    The BigInt instance (left operand).
     * @param number The built-in integer (right operand).
     * @return True if BigInt is greater than or equal to number.
     */
    QENTEM_INLINE friend bool operator>=(const BigInt &out, const Number_T number) noexcept {
        // If any higher limb is used, BigInt is greater, or equal if single limb equals number.
        return ((out.index_ != 0) || (out.storage_[0] >= number));
    }

    /**
     * @brief Compares a built-in integer with a BigInt for greater-than-or-equal.
     * @param number The built-in integer (left operand).
     * @param out    The BigInt instance (right operand).
     * @return True if number is greater than or equal to BigInt.
     */
    QENTEM_INLINE friend bool operator>=(const Number_T number, const BigInt &out) noexcept {
        // Reuse less-than-or-equal operator
        return (out <= number);
    }

    /**
     * @brief Compares BigInt with a built-in integer for equality.
     * @param out    The BigInt instance (left operand).
     * @param number The built-in integer (right operand).
     * @return True if BigInt equals number and uses only a single limb.
     */
    QENTEM_INLINE friend bool operator==(const BigInt &out, const Number_T number) noexcept {
        // Only equal if BigInt fits in one limb and matches number.
        return ((out.index_ == 0) && (out.storage_[0] == number));
    }

    /**
     * @brief Compares a built-in integer with a BigInt for equality.
     * @param number The built-in integer (left operand).
     * @param out    The BigInt instance (right operand).
     * @return True if number equals BigInt.
     */
    QENTEM_INLINE friend bool operator==(const Number_T number, const BigInt &out) noexcept {
        // Delegate to BigInt == number
        return (out == number);
    }

    /**
     * @brief Compares BigInt with a built-in integer for inequality.
     * @param out    The BigInt instance (left operand).
     * @param number The built-in integer (right operand).
     * @return True if BigInt is not equal to number or has more than one limb.
     */
    QENTEM_INLINE friend bool operator!=(const BigInt &out, const Number_T number) noexcept {
        // Not equal if higher limbs are used or value does not match number.
        return ((out.index_ != 0) || (out.storage_[0] != number));
    }

    /**
     * @brief Compares a built-in integer with a BigInt for inequality.
     * @param number The built-in integer (left operand).
     * @param out    The BigInt instance (right operand).
     * @return True if number is not equal to BigInt.
     */
    QENTEM_INLINE friend bool operator!=(const Number_T number, const BigInt &out) noexcept {
        // Delegate to BigInt != number
        return (out != number);
    }

    /**
     * @brief Shifts this BigInt left by the specified number of bits.
     * @param number Number of bits to shift.
     */
    QENTEM_INLINE void operator<<=(const SizeT32 number) noexcept {
        ShiftLeft(number);
    }

    /**
     * @brief Shifts this BigInt right by the specified number of bits.
     * @param number Number of bits to shift.
     */
    QENTEM_INLINE void operator>>=(const SizeT32 number) noexcept {
        ShiftRight(number);
    }

    /**
     * @brief Bitwise OR assignment with a built-in integer.
     * @tparam N_Number_T Any integer type (usually unsigned).
     * @param number Value to OR with.
     */
    template <typename N_Number_T>
    QENTEM_INLINE void operator|=(const N_Number_T number) noexcept {
        doOperation<BigIntOperation::Or>(number);
    }

    /**
     * @brief Bitwise AND assignment with a built-in integer.
     * @tparam N_Number_T Any integer type (usually unsigned).
     * @param number Value to AND with.
     */
    template <typename N_Number_T>
    QENTEM_INLINE void operator&=(const N_Number_T number) noexcept {
        doOperation<BigIntOperation::And>(number);
    }

    /**
     * @brief Adds a built-in integer to this BigInt (in place).
     * @tparam N_Number_T Any integer type (usually unsigned).
     * @param number Value to add.
     */
    template <typename N_Number_T>
    QENTEM_INLINE void operator+=(const N_Number_T number) noexcept {
        doOperation<BigIntOperation::Add>(number);
    }

    /**
     * @brief Subtracts a built-in integer from this BigInt (in place).
     * @tparam N_Number_T Any integer type (usually unsigned).
     * @param number Value to subtract.
     */
    template <typename N_Number_T>
    QENTEM_INLINE void operator-=(const N_Number_T number) noexcept {
        doOperation<BigIntOperation::Subtract>(number);
    }

    /**
     * @brief Multiplies this BigInt by a built-in integer (in place).
     * @param number Multiplier.
     */
    QENTEM_INLINE void operator*=(const Number_T number) noexcept {
        Multiply(number);
    }

    /**
     * @brief Divides this BigInt by a built-in integer (in place).
     * @param number Divisor.
     */
    QENTEM_INLINE void operator/=(const Number_T number) noexcept {
        Divide(number);
    }

    /**
     * @brief Adds a built-in integer value to this BigInt, starting at a given limb index.
     * @param number The value to add.
     * @param index  The starting limb index (default: 0).
     *
     * Adds the number to the specified limb, propagating carry as needed.
     * If the addition overflows a limb, carry is added to the next limb.
     * Updates the internal limb count if necessary.
     */
    void Add(Number_T number, SizeT32 index = 0) noexcept {
        if (number != 0) {
            // Propagate the addition and any carry across limbs
            while (index <= MaxIndex()) {
                const Number_T tmp = storage_[index];
                storage_[index] += number;

                // If no overflow occurred, addition is complete
                if (storage_[index] > tmp) {
                    break;
                }

                // Overflow occurred: set number to 1 to propagate carry
                number = Number_T{1};
                ++index;
            }

            // If we overflowed past the last limb, reset index_
            if (index > MaxIndex()) {
                index_ = 0;
            }
            // Otherwise, update the highest used limb index if necessary
            else if (index > index_) {
                index_ = index;
            }
        }
    }

    /**
     * @brief Subtracts a built-in integer value from this BigInt, starting at a given limb index.
     * @param number The value to subtract.
     * @param index  The starting limb index (default: 0).
     *
     * Subtracts the number from the specified limb, propagating borrow as needed.
     * If the subtraction underflows a limb, a borrow is taken from the next limb.
     * Updates the internal limb count if higher limbs become zero.
     */
    void Subtract(Number_T number, SizeT32 index = 0) noexcept {
        if (number != 0) {
            // Propagate the subtraction and any borrow across limbs
            while (index <= MaxIndex()) {
                const Number_T tmp = storage_[index];
                storage_[index] -= number;

                // If no underflow occurred, subtraction is complete
                if (storage_[index] < tmp) {
                    break;
                }

                // Underflow occurred: set number to 1 to propagate borrow
                number = Number_T{1};
                ++index;
            }

            // If borrow exceeded highest limb, set index_ to the max index
            if (index > MaxIndex()) {
                index_ = MaxIndex();
            }
            // Otherwise, trim trailing zero limbs if any
            else if (index >= index_) {
                while ((index_ != 0) && (storage_[index_] == 0)) {
                    --index_;
                }
            }
        }
    }

    /**
     * @brief Multiplies this BigInt by a built-in integer value (in place).
     * @param multiplier The value to multiply by.
     *
     * Each limb is multiplied by the multiplier, and any overflow is
     * propagated and added to the next higher limb.
     */
    QENTEM_INLINE void Multiply(Number_T multiplier) noexcept {
        // Start from the highest limb and propagate carries to higher limbs.
        SizeT32 index = index_;
        ++index;

        do {
            --index;
            // Multiply the current limb; Add() will handle carry propagation.
            Add(DoubleWidthArithmetic<Number_T, BitWidth()>::Multiply(storage_[index], multiplier), (index + 1U));
        } while (index != 0);
    }

    /**
     * @brief Divides this BigInt by a built-in integer value (in place).
     * @param divisor The value to divide by.
     * @return The remainder of the division.
     *
     * Performs long division of the entire BigInt by the given divisor.
     * Each limb is divided, propagating the remainder to lower limbs.
     * After division, index_ is updated to reflect any leading zeros.
     */
    Number_T Divide(const Number_T divisor) noexcept {
        // True if the limb size is 64 bits (affects shift optimization)
        constexpr bool is_size_64b = (BitWidth() == 64U);

        // Start from the highest limb
        SizeT32  index     = index_;
        Number_T remainder = (storage_[index_] % divisor);

        // Divide the highest limb and get the initial remainder
        storage_[index_] /= divisor;

        // Calculate the initial shift for 64-bit division optimization
        const SizeT32 initial_shift = [=]() noexcept -> SizeT32 {
            if (is_size_64b) {
                // Optimize for 64-bit limb division
                return ((BitWidth() - 1U) - Platform::FindLastBit(divisor));
            }

            return 0;
        }();

        // Process all lower limbs, propagating remainder as needed
        while (index != 0) {
            --index;
            DoubleWidthArithmetic<Number_T, BitWidth()>::Divide(remainder, storage_[index], divisor, initial_shift);
        }

        // Update the number of used limbs if highest limb is now zero
        index_ -= SizeT32((index_ != 0) && (storage_[index_] == 0));

        // Return the final remainder
        return remainder;
    }

    /**
     * @brief Shifts this BigInt right by the specified number of bits.
     * @param offset Number of bits to shift.
     *
     * Bits are shifted right across all limbs, and higher zero limbs are trimmed.
     */
    void ShiftRight(SizeT32 offset) noexcept {
        // If shifting more than one limb, move limb contents down
        if (offset >= BitWidth()) {
            SizeT32 move = (offset / BitWidth()); // Number of limbs to shift
            offset -= (move * BitWidth());        // Remaining bits to shift within a limb

            if (move <= index_) {
                // Move remaining limbs down
                SizeT32 index = 0;
                SizeT32 next  = (index + move);

                do {
                    storage_[index] = storage_[next];
                    ++index;
                    ++next;
                } while (next <= index_);

                // Zero out shifted-off high limbs
                do {
                    storage_[index_] = 0;
                    --index_;
                    --move;
                } while (move != 0);
            } else {
                // If shifting more limbs than currently used, result is zero
                Clear();
                return;
            }
        }

        // If any remaining bits to shift, do a partial shift in each limb
        if (offset != 0) {
            SizeT32       index      = 0;
            const SizeT32 shift_size = (BitWidth() - offset);

            // Shift first limb
            storage_[0] >>= offset;

            // For each following limb, shift in bits from higher limb
            while (index < index_) {
                storage_[index] |= (storage_[index + 1U] << shift_size);
                ++index;
                storage_[index] >>= offset;
            }

            // Trim highest limb if it became zero
            index_ -= (SizeT32((index_ != 0) && (storage_[index_] == 0)));
        }
    }

    /**
     * @brief Shifts this BigInt left by the specified number of bits.
     * @param offset Number of bits to shift.
     *
     * Bits are shifted left across all limbs, expanding the BigInt as needed.
     * If shifting beyond the maximum storage, the result is zero.
     */
    void ShiftLeft(SizeT32 offset) noexcept {
        // If shifting by one or more full limbs, move the data up
        if (offset >= BitWidth()) {
            SizeT32 move = (offset / BitWidth()); // Number of limbs to shift
            offset -= (move * BitWidth());        // Remaining bits to shift within a limb

            SizeT32 index = index_;
            index += move;

            // If the shift would exceed storage, clear the value
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

            // Move each limb up by 'move' positions, starting from the top
            storage_[index_ + move] = storage_[index_];

            while (index_ != 0) {
                --index_;
                storage_[index_ + move] = storage_[index_];
            }

            // Zero out the new lower limbs
            do {
                --move;
                storage_[move] = 0;
            } while (move != 0);

            // Update the highest used limb index
            while (storage_[index] == 0) {
                --index;
            }

            index_ = index;
        }

        // If any remaining bits to shift, do a partial shift in each limb
        if (offset != 0) {
            SizeT32        index      = index_;
            const SizeT32  shift_size = (BitWidth() - offset);
            const Number_T carry      = (storage_[index] >> shift_size);

            // Shift the highest limb left
            storage_[index] <<= offset;

            // If carry bits spilled over, extend limb count and add carry
            if (index_ != MaxIndex()) {
                index_ += SizeT32(carry != 0);
                storage_[index_] |= carry;
            }

            // For each lower limb, propagate bits from the lower limb up
            while (index != 0) {
                storage_[index] |= (storage_[index - 1U] >> shift_size);
                --index;
                storage_[index] <<= offset;
            }
        }
    }

    /**
     * @brief Finds the index of the first set bit (least significant) in this BigInt.
     * @return The bit position of the first set bit (starting from 0), or the last bit if all are zero.
     *
     * Scans from the lowest limb upwards and returns the bit index of the first non-zero bit.
     */
    QENTEM_INLINE SizeT32 FindFirstBit() const noexcept {
        SizeT32 index = 0;

        // Skip zero limbs from the least significant end
        while ((storage_[index] == 0) && (index <= index_)) {
            ++index;
        }

        // Use platform-specific method to find bit in the first non-zero limb
        // Bit index is (limb index * bits per limb) + offset within limb
        return (Platform::FindFirstBit(storage_[index_]) + (index * BitWidth()));
    }

    /**
     * @brief Finds the index of the last set bit (most significant) in this BigInt.
     * @return The bit position of the most significant set bit.
     *
     * Uses the highest non-zero limb and computes the position of its highest set bit.
     */
    QENTEM_INLINE SizeT32 FindLastBit() const noexcept {
        // Bit index is (limb index * bits per limb) + offset within limb
        return (Platform::FindLastBit(storage_[index_]) + SizeT32(index_ * BitWidth()));
    }

    ////////////////////////////////////////////////////
    /**
     * @brief Returns the value of the least significant limb as a built-in integer.
     * @return The lowest limb value.
     *
     * This does not convert the full BigInt to a built-in type; it just returns storage_[0].
     */
    QENTEM_INLINE Number_T Number() const noexcept {
        return storage_[0];
    }

    /**
     * @brief Sets this BigInt to zero.
     *
     * Clears all limbs and resets the index.
     */
    QENTEM_INLINE void Clear() noexcept {
        // Zero out all used limbs from highest down to zero
        while (index_ != 0) {
            storage_[index_] = 0;
            --index_;
        }

        storage_[0] = 0;
    }

    /**
     * @brief Returns the current highest limb index in use.
     * @return The highest non-zero limb index.
     */
    QENTEM_INLINE SizeT32 Index() const noexcept {
        return index_;
    }

    /**
     * @brief Returns the maximum valid limb index for this BigInt.
     * @return The maximum index into the storage array.
     *
     * Calculated as (TotalBitWidth / bits per limb) - 1.
     */
    QENTEM_INLINE static constexpr SizeT32 MaxIndex() noexcept {
        return ((TotalBitWidth() / BitWidth()) - 1U);
    }

    /**
     * @brief Returns the number of bits in each limb.
     * @return Bits per limb (e.g., 64 for uint64_t).
     */
    QENTEM_INLINE static constexpr SizeT32 BitWidth() noexcept {
        return (ByteWidth() * 8U);
    }

    /**
     * @brief Returns the total bit width of this BigInt.
     * @return Total number of bits represented.
     *
     * If Width_T is not an exact multiple of BitWidth(), rounds up to the next full limb.
     */
    QENTEM_INLINE static constexpr SizeT32 TotalBitWidth() noexcept {
        // If Width_T is a multiple of BitWidth(), just return Width_T.
        // Otherwise, add another limb to cover the excess bits.
        return (((BitWidth() * (Width_T / BitWidth())) == Width_T)
                    ? Width_T
                    : ((BitWidth() * (Width_T / BitWidth())) + BitWidth()));

        // Note: In C++, (X / Y) * Y may not equal X due to integer division truncation.
    }

    /**
     * @brief Returns the size of the limb type in bytes.
     * @return Size of Number_T in bytes.
     */
    QENTEM_INLINE static constexpr SizeT32 ByteWidth() noexcept {
        return sizeof(Number_T);
    }

    /**
     * @brief Checks if this BigInt uses more than one limb.
     * @return True if any higher limb is non-zero.
     */
    QENTEM_INLINE bool IsBig() const noexcept {
        return (index_ != 0);
    }

    /**
     * @brief Checks if this BigInt is not zero.
     * @return True if value is not zero.
     */
    QENTEM_INLINE bool NotZero() const noexcept {
        return (*this != 0);
    }

    /**
     * @brief Checks if this BigInt is zero.
     * @return True if value is zero.
     */
    QENTEM_INLINE bool IsZero() const noexcept {
        return (*this == 0);
    }

    /**
     * @brief Sets the highest limb index in use.
     * @param index The new highest index.
     *
     * This function does not zero any unused higher limbs.
     */
    QENTEM_INLINE void SetIndex(SizeT32 index) noexcept {
        index_ = index;
    }

    /**
     * @brief Returns a mutable pointer to the underlying storage array.
     * @return Pointer to storage_ array.
     */
    QENTEM_INLINE Number_T *Storage() noexcept {
        return storage_;
    }

    /**
     * @brief Returns a const pointer to the underlying storage array.
     * @return Const pointer to storage_ array.
     */
    QENTEM_INLINE const Number_T *Storage() const noexcept {
        return storage_;
    }

  private:
    /**
     * @brief Copies the value from another BigInt.
     * @param src The source BigInt to copy from.
     *
     * Copies all active limbs and zeros out any unused higher limbs.
     */
    void copy(const BigInt &src) noexcept {
        SizeT32 index = 0;

        // Copy each used limb from source
        while (index <= src.index_) {
            storage_[index] = src.storage_[index];
            ++index;
        }

        // Zero out any extra limbs from previous state
        while (index_ > index) {
            storage_[index_] = 0;
            --index_;
        }

        // Update current highest index in use
        index_ = src.index_;
    }

    /**
     * @brief Internal helper to perform an arithmetic or bitwise operation on this BigInt and a built-in integer.
     * @tparam Operation The operation to perform (Set, Or, And, Add, Subtract).
     * @param number The value to use in the operation.
     *
     * Used by assignment and compound operators for efficient, in-place modification.
     */
    template <BigIntOperation Operation>
    QENTEM_INLINE void doOperation(Number_T number) noexcept {
        switch (Operation) {
            case BigIntOperation::Set: {
                // Assign a new value to this BigInt, clearing all higher limbs.
                storage_[0] = number;
                index_      = 0;
                break;
            }

            case BigIntOperation::Or: {
                // Bitwise OR with the lowest limb only.
                storage_[0] |= number;
                break;
            }

            case BigIntOperation::And: {
                // Bitwise AND with the lowest limb only; higher limbs are cleared.
                storage_[0] &= number;
                index_ = 0;
                break;
            }

            case BigIntOperation::Add: {
                // Add the number to this BigInt.
                Add(number);
                break;
            }

            case BigIntOperation::Subtract: {
                // Subtract the number from this BigInt.
                Subtract(number);
                break;
            }
        }
    }

    /**
     * @brief Internal helper to perform an arithmetic or bitwise operation on this BigInt and a (possibly wider)
     * integer type.
     * @tparam Operation The operation to perform (Set, Or, And, Add, Subtract).
     * @tparam N_Number_T The input type, may be wider than the BigInt limb type.
     * @param number The value to use in the operation.
     *
     * Handles wide integer input by operating on each limb-sized chunk.
     * Used by assignment and compound operators for in-place modification.
     */
    template <BigIntOperation Operation, typename N_Number_T>
    QENTEM_INLINE void doOperation(N_Number_T number) noexcept {
        // True if N_Number_T is wider than one limb.
        constexpr bool is_bigger_size = (((sizeof(N_Number_T) * 8U) / BitWidth()) > 1U);

        // First, operate on the lowest limb.
        switch (Operation) {
            case BigIntOperation::Set: {
                // Assign lowest limb, clear higher limbs
                storage_[0] = Number_T(number);
                index_      = 0;
                break;
            }

            case BigIntOperation::Or: {
                // Bitwise OR with the lowest limb only
                storage_[0] |= Number_T(number);
                break;
            }

            case BigIntOperation::And: {
                // Bitwise AND with the lowest limb, clear higher limbs
                storage_[0] &= Number_T(number);
                index_ = 0;
                break;
            }

            case BigIntOperation::Add: {
                // Add to the lowest limb
                Add(Number_T(number));
                break;
            }

            case BigIntOperation::Subtract: {
                // Subtract from the lowest limb
                Subtract(Number_T(number));
                break;
            }
        }

        // If the input is wider than a single limb, operate on all higher limb chunks
        if constexpr (is_bigger_size) {
            SizeT32 index = 1U;
            number >>= BitWidth();

            while (number != 0) {
                switch (Operation) {
                    case BigIntOperation::Set: {
                        // Assign higher limb
                        storage_[index] = Number_T(number);
                        ++index_;
                        break;
                    }

                    case BigIntOperation::Or: {
                        // Bitwise OR with higher limb
                        storage_[index] |= Number_T(number);
                        // Update index_ if we've touched a new higher limb
                        if (index > index_) {
                            index_ = index;
                        }

                        break;
                    }

                    case BigIntOperation::And: {
                        // Bitwise AND with higher limb
                        storage_[index] &= Number_T(number);
                        // Only keep index_ high if this limb is nonzero
                        if (storage_[index] != 0) {
                            index_ = index;
                        }

                        break;
                    }

                    case BigIntOperation::Add: {
                        // Add to higher limb
                        Add(Number_T(number), index);
                        break;
                    }

                    case BigIntOperation::Subtract: {
                        // Subtract from higher limb
                        Subtract(Number_T(number), index);
                        break;
                    }
                }

                number >>= BitWidth();
                ++index;
            }
        }
    }

    /**
     * @brief Internal storage for BigInt limbs.
     *
     * Each element represents a fixed-width chunk ("limb") of the overall value.
     * Size is determined by MaxIndex() + 1 to cover all requested bits.
     */
    Number_T storage_[MaxIndex() + Number_T{1}]{0};

    /**
     * @brief Highest non-zero limb index currently used.
     */
    SizeT32 index_{0};
};

/**
 * @brief Provides 16-bit (double 8-bit) arithmetic for small-limb BigInts.
 *
 * Used internally for division and multiplication with 8-bit storage types.
 * Combines two 8-bit limbs into a 16-bit value for carry handling.
 * @tparam Number_T The base type for a single limb (e.g., uint8_t).
 */
template <typename Number_T>
struct DoubleWidthArithmetic<Number_T, 8U> {
    /**
     * @brief Divides a combined 16-bit value (dividend_high:dividend_low) by divisor.
     * @param[in,out] dividend_high The high 8 bits, replaced with the new high (remainder).
     * @param[in,out] dividend_low  The low 8 bits, replaced with the new low (quotient).
     * @param divisor  The divisor.
     * @param         (unused) Shift value, required by interface.
     *
     * Packs two 8-bit values into 16 bits, divides, then splits result.
     */
    QENTEM_INLINE static void Divide(Number_T &dividend_high, Number_T &dividend_low, const Number_T divisor,
                                     SizeT32) noexcept {
        SizeT16 dividend16{dividend_high};              // Pack high byte
        dividend16 <<= shift_;                          // Shift up by 8 bits
        dividend16 |= dividend_low;                     // Pack in low byte
        dividend_high = Number_T(dividend16 % divisor); // New remainder in high
        dividend16 /= divisor;                          // New quotient in low
        dividend_low = Number_T(dividend16);
    }

    /**
     * @brief Multiplies two 8-bit values as if they were 16 bits, updates number with lower 8 bits.
     * @param[in,out] number      The left operand, replaced with the low 8 bits of the result.
     * @param         multiplier  The right operand.
     * @return The upper 8 bits (carry) of the result.
     *
     * Used for multi-limb multiplication.
     */
    QENTEM_INLINE static Number_T Multiply(Number_T &number, const Number_T multiplier) noexcept {
        SizeT16 number16{number};            // Promote to 16 bits
        number16 *= multiplier;              // Perform 16-bit multiplication
        number = Number_T(number16);         // Store the low 8 bits in 'number'
        return Number_T(number16 >> shift_); // Return upper 8 bits as carry
    }

  private:
    static constexpr SizeT32 shift_ = 8U; ///< Number of bits per limb.
};

/**
 * @brief Provides 32-bit (double 16-bit) arithmetic for small-limb BigInts.
 *
 * Used internally for division and multiplication with 16-bit storage types.
 * Combines two 16-bit limbs into a 32-bit value for carry handling.
 * @tparam Number_T The base type for a single limb (e.g., uint16_t).
 */
template <typename Number_T>
struct DoubleWidthArithmetic<Number_T, 16U> {
    /**
     * @brief Divides a combined 32-bit value (dividend_high:dividend_low) by divisor.
     * @param[in,out] dividend_high The high 16 bits, replaced with new high (remainder).
     * @param[in,out] dividend_low  The low 16 bits, replaced with new low (quotient).
     * @param divisor  The divisor.
     * @param         (unused) Shift value, required by interface.
     *
     * Packs two 16-bit values into 32 bits, divides, then splits result.
     */
    QENTEM_INLINE static void Divide(Number_T &dividend_high, Number_T &dividend_low, const Number_T divisor,
                                     SizeT32) noexcept {
        SizeT32 dividend32{dividend_high};              // Pack high 16 bits
        dividend32 <<= shift_;                          // Shift to high word
        dividend32 |= dividend_low;                     // Add in low 16 bits
        dividend_high = Number_T(dividend32 % divisor); // New remainder in high
        dividend32 /= divisor;                          // New quotient in low
        dividend_low = Number_T(dividend32);
    }

    /**
     * @brief Multiplies two 16-bit values as if they were 32 bits, updates number with lower 16 bits.
     * @param[in,out] number      The left operand, replaced with the low 16 bits of the result.
     * @param         multiplier  The right operand.
     * @return The upper 16 bits (carry) of the result.
     *
     * Used for multi-limb multiplication.
     */
    QENTEM_INLINE static Number_T Multiply(Number_T &number, const Number_T multiplier) noexcept {
        SizeT32 number32{number};            // Promote to 32 bits
        number32 *= multiplier;              // 32-bit multiplication
        number = Number_T(number32);         // Store low 16 bits
        return Number_T(number32 >> shift_); // Return high 16 bits as carry
    }

  private:
    static constexpr SizeT32 shift_ = 16U; ///< Number of bits per limb.
};
////////////////////////////////////////////////////
/**
 * @brief Provides 64-bit (double 32-bit) arithmetic for medium-limb BigInts.
 *
 * Used internally for division and multiplication with 32-bit storage types.
 * Combines two 32-bit limbs into a 64-bit value for carry handling.
 * @tparam Number_T The base type for a single limb (e.g., uint32_t).
 */
template <typename Number_T>
struct DoubleWidthArithmetic<Number_T, 32U> {
    /**
     * @brief Divides a combined 64-bit value (dividend_high:dividend_low) by divisor.
     * @param[in,out] dividend_high The high 32 bits, replaced with the new high (remainder).
     * @param[in,out] dividend_low  The low 32 bits, replaced with the new low (quotient).
     * @param divisor  The divisor.
     * @param         (unused) Shift value, required by interface.
     *
     * Packs two 32-bit values into 64 bits, divides, then splits result.
     */
    QENTEM_INLINE static void Divide(Number_T &dividend_high, Number_T &dividend_low, const Number_T divisor,
                                     SizeT32) noexcept {
        SizeT64 dividend64{dividend_high};              // Pack high 32 bits
        dividend64 <<= shift_;                          // Shift to high dword
        dividend64 |= dividend_low;                     // Add in low 32 bits
        dividend_high = Number_T(dividend64 % divisor); // New remainder in high
        dividend64 /= divisor;                          // New quotient in low
        dividend_low = Number_T(dividend64);
    }

    /**
     * @brief Multiplies two 32-bit values as if they were 64 bits, updates number with lower 32 bits.
     * @param[in,out] number      The left operand, replaced with the low 32 bits of the result.
     * @param         multiplier  The right operand.
     * @return The upper 32 bits (carry) of the result.
     *
     * Used for multi-limb multiplication.
     */
    QENTEM_INLINE static Number_T Multiply(Number_T &number, const Number_T multiplier) noexcept {
        SizeT64 number64{number};            // Promote to 64 bits
        number64 *= multiplier;              // 64-bit multiplication
        number = Number_T(number64);         // Store low 32 bits
        return Number_T(number64 >> shift_); // Return high 32 bits as carry
    }

  private:
    static constexpr SizeT32 shift_ = 32U; ///< Number of bits per limb.
};
////////////////////////////////////////////////////
/**
 * @brief Double-width arithmetic for 64-bit limbs (128-bit intermediate math).
 *
 * This specialization is used by BigInt for division and multiplication when each limb
 * is 64 bits wide (e.g., `uint64_t`). It implements core algorithms for dividing and multiplying
 * a pair of 64-bit limbs as if they were a 128-bit value, allowing full-precision
 * multi-limb operations on hardware that does not provide native 128-bit types.
 *
 * The division algorithm is based on Knuth's Algorithm D, adapted for platforms lacking
 * intrinsic 128-bit division, and carefully handles carries, overflows, and normalization.
 *
 * @tparam Number_T The unsigned integer type for one limb (must be 64 bits wide).
 */
template <typename Number_T>
struct DoubleWidthArithmetic<Number_T, 64U> {
    /**
     * @brief Performs division of a 128-bit integer by a 64-bit divisor.
     *
     * Given dividend_high:dividend_low (forming a 128-bit integer), divides by @p divisor,
     * updating dividend_low with the quotient and dividend_high with the remainder.
     *
     * This function is critical for algorithms such as long division, Barrett reduction,
     * or modular exponentiation where 128-bit division is needed but only 64-bit types are available.
     *
     * @param[in,out] dividend_high The high 64 bits (will be replaced with the new high/remainder).
     * @param[in,out] dividend_low  The low 64 bits (will be replaced with the new low/quotient).
     * @param divisor  The 64-bit divisor.
     * @param initial_shift The normalization shift required so that divisor's high bit is set.
     *
     * Algorithm steps:
     *   1. Normalize the divisor and dividend to ensure the highest bit of the divisor is set.
     *   2. Compute the quotient and correct for possible overestimation.
     *   3. Repeat for two quotient digits (since we are doing 128 / 64 division).
     *   4. Unnormalize the remainder.
     *   5. Handle carries and overflows.
     */
    static void Divide(Number_T &dividend_high, Number_T &dividend_low, const Number_T divisor,
                       const SizeT32 initial_shift) noexcept {
        // Step 1: Save the remainder that results from dividing the low part
        const Number_T carry = (dividend_low % divisor);
        dividend_low /= divisor;

        // Step 2: Normalize the divisor so its highest bit is set
        const Number_T divisor_shifted = (divisor << initial_shift);

        // Step 3: Split divisor into high and low halves for two-quotient computation
        const Number_T divisor_low  = (divisor_shifted >> shift_); // Lower half of normalized divisor
        const Number_T divisor_high = (divisor_shifted & mask_);   // Higher half of normalized divisor

        // Step 4: Normalize dividend high part
        dividend_high <<= initial_shift;

        // Step 5: Compute first quotient digit (high half of 128 bits divided by low half of divisor)
        Number_T quotient = (dividend_high / divisor_low);
        dividend_high %= divisor_low;
        Number_T remainder = (quotient * divisor_high);

        // Step 6: Shift for next digit
        dividend_high <<= shift_;

        // Step 7: Adjust quotient if overestimated (Knuth's correction step)
        if (dividend_high < remainder) {
            --quotient;

            if ((remainder - dividend_high) > divisor_shifted) {
                --quotient;
                remainder -= divisor_shifted;
            }

            remainder -= divisor_shifted;
        }

        dividend_high -= remainder;

        // Step 8: Accumulate quotient in low part (move to lower 64 bits)
        quotient <<= shift_;
        dividend_low += quotient;

        // Step 9: Repeat process for next digit (low half of the normalized dividend)
        quotient = (dividend_high / divisor_low);
        dividend_high %= divisor_low;
        remainder = (quotient * divisor_high);

        dividend_high <<= shift_;

        // Step 10: Correction step for the new digit
        if (dividend_high < remainder) {
            --quotient;

            if ((remainder - dividend_high) > divisor_shifted) {
                --quotient;
                remainder -= divisor_shifted;
            }

            remainder -= divisor_shifted;
        }

        dividend_high -= remainder;

        // Step 11: Accumulate quotient
        dividend_low += quotient;

        // Step 12: Unnormalize remainder
        dividend_high >>= initial_shift;

        // Step 13: Add back carry from original division
        const Number_T original_dividend_high = dividend_high;
        dividend_high += carry;

        // Step 14: If adding carry overflowed, correct the overflow in both parts
        if (original_dividend_high > dividend_high) {
            // Overflow: need to increment quotient
            constexpr Number_T overflow_dividend = (Number_T{1} << (width_ - 1U));

            dividend_high += ((overflow_dividend % (divisor >> 1U)) << 1U);
            ++dividend_low;
        }

        // Step 15: If dividend_high >= divisor, reduce and increment low
        if (dividend_high >= divisor) {
            dividend_high -= divisor;
            ++dividend_low;
        }
    }

    /**
     * @brief Performs 128-bit multiplication (two 64-bit limbs).
     *
     * Multiplies @p number by @p multiplier, updating @p number with the lower 64 bits
     * and returning the upper 64 bits as the carry/overflow.
     *
     * Splits both operands into high/low 32-bit halves to prevent overflow and
     * assembles the result in two steps.
     *
     * @param[in,out] number The left operand (will hold the low result).
     * @param multiplier The right operand.
     * @return The high 64 bits of the result.
     *
     * Steps:
     *   1. Multiply lower halves and set the low part.
     *   2. Accumulate all cross-products in the correct positions.
     *   3. Return the high part as the carry.
     */
    static Number_T Multiply(Number_T &number, Number_T multiplier) noexcept {
        // Split operands into low and high 32-bit parts
        const Number_T number_low     = (number & mask_);
        Number_T       number_high    = number;
        Number_T       multiplier_low = (multiplier & mask_);

        // Multiply low parts; store in 'number' (the low 64 bits)
        number = (number_low * multiplier_low);

        // Compute cross products and accumulate their carries
        number_high >>= shift_;
        multiplier_low *= number_high;        // Cross: number_high * multiplier_low
        multiplier_low += (number >> shift_); // Add upper half of current low product
        number &= mask_;                      // Mask out the lower part (keep 64 bits)

        // Prepare for next cross product
        multiplier >>= shift_;
        number_high *= multiplier;                 // Cross: number_high * multiplier_high
        number_high += (multiplier_low >> shift_); // Add cross-product carry

        multiplier_low &= mask_;                     // Keep only low part
        multiplier_low += (number_low * multiplier); // Cross: number_low * multiplier_high

        // Reassemble: store cross products into result
        number |= (multiplier_low << shift_);
        multiplier_low >>= shift_;
        number_high += multiplier_low;

        return number_high; // Return upper 64 bits
    }

  private:
    static constexpr SizeT32  width_ = (sizeof(Number_T) * 8U);    ///< Total bits in a limb.
    static constexpr SizeT32  shift_ = (width_ / 2U);              ///< Shift for half-width partitioning.
    static constexpr Number_T mask_  = (~(Number_T{0}) >> shift_); ///< Mask for extracting lower half.
};

} // namespace Qentem

#endif
