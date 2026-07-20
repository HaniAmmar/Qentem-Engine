/**
 * @file BigInt.hpp
 * @brief Arbitrary-precision unsigned integer arithmetic for Qentem Engine.
 *
 * This header provides the BigInt class template, a portable arbitrary-precision
 * integer implementation for applications that require integer values larger than
 * the native machine word size.
 *
 * BigInt uses a limb-based representation and implements the core arithmetic
 * operations required for high-precision computation, including addition,
 * subtraction, multiplication, division, shifting, comparison, and bitwise
 * manipulation.
 *
 * Typical Use Cases:
 * - Accurate floating-point to string conversion
 * - Arbitrary-precision integer arithmetic
 * - Number theory and mathematical algorithms
 * - Cryptographic and finite-field arithmetic
 * - Modular exponentiation and modular inversion
 * - Barrett and Montgomery reduction
 * - Primality testing (Miller-Rabin, Fermat, etc.)
 * - Rational, fixed-point, and decimal numeric types
 * - Parsing and encoding large numeric values
 * - Protocols and file formats containing large integer fields
 * - Scientific, engineering, and symbolic computation
 *
 * Features:
 * - Configurable unsigned limb type (e.g. uint8_t, uint16_t, uint32_t, uint64_t)
 * - Arbitrary-precision addition, subtraction, multiplication, and division
 * - Quotient and remainder computation
 * - Bitwise AND, OR, shifts, and comparison operations
 * - Deterministic arithmetic behavior across supported platforms
 *
 * @tparam Number_T Unsigned integer type used as the storage limb.
 * @tparam Bits     Maximum number of bits available for the integer value.
 *
 * @author Hani Ammar
 * @date 2026
 * @copyright MIT License
 *
 * @note This implementation operates on unsigned integer limbs. Signed integer
 *       support can be implemented as a higher-level abstraction.
 * @note For best performance on modern systems, 32-bit or 64-bit limb types are
 *       generally recommended.
 *
 * @usage
 *     #include "Qentem/BigInt.hpp"
 *     using Qentem::BigInt;
 *
 *     // 128-bit unsigned integer using 64-bit limbs.
 *     BigInt<unsigned long long, 128U> value = 0xFFFFFFFFFFFFFFFFULL;
 *     value <<= 64U;
 *     value |= 0xFFFFFFFFFFFFFFFFULL;
 */

#ifndef QENTEM_BIG_INT_H
#define QENTEM_BIG_INT_H

#include "Qentem/Platform.hpp"

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
    BigInt() noexcept {
        SizeT32 index = 0;

        while (index <= MaxIndex()) {
            Storage()[index] = 0;
            ++index;
        }
    }

    /**
     * @brief Destructor.
     */
    QENTEM_INLINE ~BigInt() noexcept = default;

    /**
     * @brief Move constructor.
     * @param src Source BigInt. Its contents are cleared after construction.
     */
    BigInt(BigInt &&src) noexcept : index_{src.index_} {
        SizeT32 index = 0;
        // Copy all active limbs from src to this BigInt
        while (index <= index_) {
            storage_[index] = src.storage_[index];
            ++index;
        }

        while (index <= MaxIndex()) {
            Storage()[index] = 0;
            ++index;
        }

        src.Clear();
    }

    /**
     * @brief Copy constructor.
     * @param src Source BigInt to copy from.
     */
    BigInt(const BigInt &src) noexcept : index_{src.index_} {
        SizeT32 index = 0;
        // Copy all limbs from src to this BigInt
        while (index <= index_) {
            storage_[index] = src.storage_[index];
            ++index;
        }

        while (index <= MaxIndex()) {
            Storage()[index] = 0;
            ++index;
        }
    }

    /**
     * @brief Construct BigInt from a built-in integer type.
     * @tparam N_Number_T Any integer type (usually unsigned).
     * @param number Value to initialize with.
     */
    template <typename N_Number_T>
    BigInt(const N_Number_T number) noexcept : index_{0} {
        SizeT32 index = 0;

        while (index <= MaxIndex()) {
            Storage()[index] = 0;
            ++index;
        }

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
    BigInt &operator=(const N_Number_T number) noexcept {
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
    BigInt &operator=(BigInt &&src) noexcept {
        if (this != &src) {
            Copy(src);
            src.Clear();
        }

        return *this;
    }

    /**
     * @brief Copy assignment operator.
     * @param src Source BigInt to copy from.
     * @return Reference to this BigInt.
     */
    BigInt &operator=(const BigInt &src) noexcept {
        if (this != &src) {
            Copy(src);
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
     * @param left   The BigInt instance (left operand).
     * @param number The built-in integer (right operand).
     * @return True if BigInt is less than number.
     *
     * Only true if the BigInt fits in a single limb.
     */
    QENTEM_INLINE friend bool operator<(const BigInt &left, const Number_T number) noexcept {
        // If BigInt has only one limb, compare directly.
        return ((left.index_ == 0) && (left.storage_[0] < number));
    }

    /**
     * @brief Compares a built-in integer with a BigInt for less-than.
     * @param number The built-in integer (left operand).
     * @param right    The BigInt instance (right operand).
     * @return True if number is less than BigInt.
     */
    QENTEM_INLINE friend bool operator<(const Number_T number, const BigInt &right) noexcept {
        // Reuse the greater-than operator
        return (right > number);
    }

    /**
     * @brief Checks whether a BigInt value is less than another.
     *
     * Compares the number of used limbs first. If the limb counts differ,
     * the value with fewer used limbs is considered smaller. Otherwise, the
     * limbs are compared from the most significant limb down to the least
     * significant limb until a difference is found.
     *
     * @param left Left-hand BigInt operand.
     * @param right Right-hand BigInt operand.
     *
     * @return True if @p left is less than @p right; otherwise, false.
     */
    QENTEM_NOINLINE friend bool operator<(const BigInt &left, const BigInt &right) noexcept {
        return IsLess(left, right);
    }

    /**
     * @brief Checks whether a BigInt value is less than another.
     *
     * Compares the number of used limbs first. If the limb counts differ,
     * the value with fewer used limbs is considered smaller. Otherwise, the
     * limbs are compared from the most significant limb down to the least
     * significant limb until a difference is found.
     *
     * @tparam left Left-hand BigInt operand.
     * @tparam right Right-hand BigInt operand.
     *
     * @return True if @p left is less than @p right; otherwise, false.
     */
    template <typename BigInt_Left_T, typename BigInt_Right_T>
    QENTEM_NOINLINE static bool IsLess(const BigInt_Left_T &left, const BigInt_Right_T &right) noexcept {
        if (left.Index() != right.Index()) {
            return (left.Index() < right.Index());
        }

        SizeT32 index = left.Index();

        while (index != 0) {
            if (left.Storage()[index] == right.Storage()[index]) {
                --index;
                continue;
            }

            return (left.Storage()[index] < right.Storage()[index]);
        }

        return (left.Storage()[index] < right.Storage()[index]);
    }

    /**
     * @brief Compares BigInt with a built-in integer for less-than-or-equal.
     * @param left    The BigInt instance (left operand).
     * @param number The built-in integer (right operand).
     * @return True if BigInt is less than or equal to number.
     */
    QENTEM_INLINE friend bool operator<=(const BigInt &left, const Number_T number) noexcept {
        // If BigInt has only one limb, compare directly.
        return ((left.index_ == 0) && (left.storage_[0] <= number));
    }

    /**
     * @brief Compares a built-in integer with a BigInt for less-than-or-equal.
     * @param number The built-in integer (left operand).
     * @param right    The BigInt instance (right operand).
     * @return True if number is less than or equal to BigInt.
     */
    QENTEM_INLINE friend bool operator<=(const Number_T number, const BigInt &right) noexcept {
        // Reuse the greater-than-or-equal operator
        return (right >= number);
    }

    /**
     * @brief Checks whether a BigInt value is less than or equal to another.
     *
     * Compares the number of used limbs first. If the limb counts differ,
     * the value with fewer used limbs is considered smaller. Otherwise, the
     * limbs are compared from the most significant limb down to the least
     * significant limb until a difference is found.
     *
     * @param left Left-hand BigInt operand.
     * @param right Right-hand BigInt operand.
     *
     * @return True if @p left is less than or equal to @p right;
     *         otherwise, false.
     */
    QENTEM_NOINLINE friend bool operator<=(const BigInt &left, const BigInt &right) noexcept {
        return IsLessOrEqual(left, right);
    }

    /**
     * @brief Checks whether a BigInt value is less than or equal to another.
     *
     * Compares the number of used limbs first. If the limb counts differ,
     * the value with fewer used limbs is considered smaller. Otherwise, the
     * limbs are compared from the most significant limb down to the least
     * significant limb until a difference is found.
     *
     * @tparam left Left-hand BigInt operand.
     * @tparam right Right-hand BigInt operand.
     *
     * @return True if @p left is less than or equal to @p right;
     *         otherwise, false.
     */
    template <typename BigInt_Left_T, typename BigInt_Right_T>
    QENTEM_NOINLINE static bool IsLessOrEqual(const BigInt_Left_T &left, const BigInt_Right_T &right) noexcept {
        if (left.Index() != right.Index()) {
            return (left.Index() < right.Index());
        }

        SizeT32 index = left.Index();

        while (index != 0) {
            if (left.Storage()[index] == right.Storage()[index]) {
                --index;
                continue;
            }

            return (left.Storage()[index] < right.Storage()[index]);
        }

        return (left.Storage()[index] <= right.Storage()[index]);
    }

    /**
     * @brief Compares BigInt with a built-in integer for greater-than.
     * @param left    The BigInt instance (left operand).
     * @param number The built-in integer (right operand).
     * @return True if BigInt is greater than number.
     *
     * True if BigInt has more than one limb or the value is greater than number.
     */
    QENTEM_INLINE friend bool operator>(const BigInt &left, const Number_T number) noexcept {
        // If any higher limb is used, BigInt is greater.
        return ((left.index_ != 0) || (left.storage_[0] > number));
    }

    /**
     * @brief Compares a built-in integer with a BigInt for greater-than.
     * @param number The built-in integer (left operand).
     * @param right    The BigInt instance (right operand).
     * @return True if number is greater than BigInt.
     */
    QENTEM_INLINE friend bool operator>(const Number_T number, const BigInt &right) noexcept {
        // Reuse less-than operator
        return (right < number);
    }

    /**
     * @brief Checks whether a BigInt value is greater than another.
     *
     * Compares the number of used limbs first. If the limb counts differ,
     * the value with more used limbs is considered greater. Otherwise, the
     * limbs are compared from the most significant limb down to the least
     * significant limb until a difference is found.
     *
     * @param left Left-hand BigInt operand.
     * @param right Right-hand BigInt operand.
     *
     * @return True if @p left is greater than @p right; otherwise, false.
     */
    QENTEM_NOINLINE friend bool operator>(const BigInt &left, const BigInt &right) noexcept {
        return IsGreater(left, right);
    }

    /**
     * @brief Checks whether a BigInt value is greater than another.
     *
     * Compares the number of used limbs first. If the limb counts differ,
     * the value with more used limbs is considered greater. Otherwise, the
     * limbs are compared from the most significant limb down to the least
     * significant limb until a difference is found.
     *
     * @tparam left Left-hand BigInt operand.
     * @tparam right Right-hand BigInt operand.
     *
     * @return True if @p left is greater than @p right; otherwise, false.
     */
    template <typename BigInt_Left_T, typename BigInt_Right_T>
    QENTEM_NOINLINE static bool IsGreater(const BigInt_Left_T &left, const BigInt_Right_T &right) noexcept {
        if (left.Index() != right.Index()) {
            return (left.Index() > right.Index());
        }

        SizeT32 index = left.Index();

        while (index != 0) {
            if (left.Storage()[index] == right.Storage()[index]) {
                --index;
                continue;
            }

            return (left.Storage()[index] > right.Storage()[index]);
        }

        return (left.Storage()[index] > right.Storage()[index]);
    }

    /**
     * @brief Compares BigInt with a built-in integer for greater-than-or-equal.
     * @param left    The BigInt instance (left operand).
     * @param number The built-in integer (right operand).
     * @return True if BigInt is greater than or equal to number.
     */
    QENTEM_INLINE friend bool operator>=(const BigInt &left, const Number_T number) noexcept {
        // If any higher limb is used, BigInt is greater, or equal if single limb equals number.
        return ((left.index_ != 0) || (left.storage_[0] >= number));
    }

    /**
     * @brief Compares a built-in integer with a BigInt for greater-than-or-equal.
     * @param number The built-in integer (left operand).
     * @param right    The BigInt instance (right operand).
     * @return True if number is greater than or equal to BigInt.
     */
    QENTEM_INLINE friend bool operator>=(const Number_T number, const BigInt &right) noexcept {
        // Reuse less-than-or-equal operator
        return (right <= number);
    }

    /**
     * @brief Checks whether a BigInt value is greater than or equal to another.
     *
     * Compares the number of used limbs first. If the limb counts differ,
     * the value with more used limbs is considered greater. Otherwise, the
     * limbs are compared from the most significant limb down to the least
     * significant limb until a difference is found.
     *
     * @param left Left-hand BigInt operand.
     * @param right Right-hand BigInt operand.
     *
     * @return True if @p left is greater than or equal to @p right;
     *         otherwise, false.
     */
    QENTEM_NOINLINE friend bool operator>=(const BigInt &left, const BigInt &right) noexcept {
        return IsGreaterOrEqual(left, right);
    }

    /**
     * @brief Checks whether a BigInt value is greater than or equal to another.
     *
     * Compares the number of used limbs first. If the limb counts differ,
     * the value with more used limbs is considered greater. Otherwise, the
     * limbs are compared from the most significant limb down to the least
     * significant limb until a difference is found.
     *
     * @tparam left Left-hand BigInt operand.
     * @tparam right Right-hand BigInt operand.
     *
     * @return True if @p left is greater than or equal to @p right;
     *         otherwise, false.
     */
    template <typename BigInt_Left_T, typename BigInt_Right_T>
    QENTEM_NOINLINE static bool IsGreaterOrEqual(const BigInt_Left_T &left, const BigInt_Right_T &right) noexcept {
        if (left.Index() != right.Index()) {
            return (left.Index() > right.Index());
        }

        SizeT32 index = left.Index();

        while (index != 0) {
            if (left.Storage()[index] == right.Storage()[index]) {
                --index;
                continue;
            }

            return (left.Storage()[index] > right.Storage()[index]);
        }

        return (left.Storage()[index] >= right.Storage()[index]);
    }

    /**
     * @brief Compares BigInt with a built-in integer for equality.
     * @param left    The BigInt instance (left operand).
     * @param number The built-in integer (right operand).
     * @return True if BigInt equals number and uses only a single limb.
     */
    QENTEM_INLINE friend bool operator==(const BigInt &left, const Number_T number) noexcept {
        // Only equal if BigInt fits in one limb and matches number.
        return ((left.index_ == 0) && (left.storage_[0] == number));
    }

    /**
     * @brief Compares a built-in integer with a BigInt for equality.
     * @param number The built-in integer (left operand).
     * @param right    The BigInt instance (right operand).
     * @return True if number equals BigInt.
     */
    QENTEM_INLINE friend bool operator==(const Number_T number, const BigInt &right) noexcept {
        // Delegate to BigInt == number
        return (right == number);
    }

    /**
     * @brief Checks whether two BigInt values are equal.
     *
     * Compares the number of used limbs first. If the limb counts differ,
     * the values are considered unequal. Otherwise, all limbs are compared
     * from the most significant limb down to the least significant limb.
     *
     * @param left Left-hand BigInt operand.
     * @param right Right-hand BigInt operand.
     *
     * @return True if the values are equal; otherwise, false.
     */
    QENTEM_NOINLINE friend bool operator==(const BigInt &left, const BigInt &right) noexcept {
        return IsEqual(left, right);
    }

    /**
     * @brief Checks whether two BigInt values are equal.
     *
     * Compares the number of used limbs first. If the limb counts differ,
     * the values are considered unequal. Otherwise, all limbs are compared
     * from the most significant limb down to the least significant limb.
     *
     * @tparam left Left-hand BigInt operand.
     * @tparam right Right-hand BigInt operand.
     *
     * @return True if the values are equal; otherwise, false.
     */
    template <typename BigInt_Left_T, typename BigInt_Right_T>
    QENTEM_NOINLINE static bool IsEqual(const BigInt_Left_T &left, const BigInt_Right_T &right) noexcept {
        if (left.Index() != right.Index()) {
            return false;
        }

        SizeT32 index = left.Index();

        while (index != 0) {
            if (left.Storage()[index] != right.Storage()[index]) {
                return false;
            }

            --index;
        }

        return (left.Storage()[0] == right.Storage()[0]);
    }

    /**
     * @brief Compares BigInt with a built-in integer for inequality.
     * @param left    The BigInt instance (left operand).
     * @param number The built-in integer (right operand).
     * @return True if BigInt is not equal to number or has more than one limb.
     */
    QENTEM_INLINE friend bool operator!=(const BigInt &left, const Number_T number) noexcept {
        // Not equal if higher limbs are used or value does not match number.
        return ((left.index_ != 0) || (left.storage_[0] != number));
    }

    /**
     * @brief Compares a built-in integer with a BigInt for inequality.
     * @param number The built-in integer (left operand).
     * @param right    The BigInt instance (right operand).
     * @return True if number is not equal to BigInt.
     */
    QENTEM_INLINE friend bool operator!=(const Number_T number, const BigInt &right) noexcept {
        // Delegate to BigInt != number
        return (right != number);
    }

    /**
     * @brief Checks whether two BigInt values are not equal.
     *
     * Compares the number of used limbs first. If the limb counts differ,
     * the values are considered unequal. Otherwise, all limbs are compared
     * from the most significant limb down to the least significant limb.
     *
     * @param left Left-hand BigInt operand.
     * @param right Right-hand BigInt operand.
     *
     * @return True if the values are not equal; otherwise, false.
     */
    QENTEM_NOINLINE friend bool operator!=(const BigInt &left, const BigInt &right) noexcept {
        return IsNotEqual(left, right);
    }

    /**
     * @brief Checks whether two BigInt values are not equal.
     *
     * Compares the number of used limbs first. If the limb counts differ,
     * the values are considered unequal. Otherwise, all limbs are compared
     * from the most significant limb down to the least significant limb.
     *
     * @tparam left Left-hand BigInt operand.
     * @tparam right Right-hand BigInt operand.
     *
     * @return True if the values are not equal; otherwise, false.
     */
    template <typename BigInt_Left_T, typename BigInt_Right_T>
    QENTEM_NOINLINE static bool IsNotEqual(const BigInt_Left_T &left, const BigInt_Right_T &right) noexcept {
        if (left.Index() != right.Index()) {
            return true;
        }

        SizeT32 index = left.Index();

        while (index != 0) {
            if (left.Storage()[index] != right.Storage()[index]) {
                return true;
            }

            --index;
        }

        return (left.Storage()[index] != right.Storage()[index]);
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
     * @brief Performs a bitwise OR operation with another BigInt type.
     *
     * Updates this BigInt in place by applying a bitwise OR between each
     * corresponding storage element of this object and @p bint.
     *
     * Only storage elements up to the smaller of the two active indices are
     * processed. Any higher storage elements already present in this object remain
     * unchanged.
     *
     * @tparam BigInt_T A BigInt.
     * @param bint The value to OR with this BigInt.
     *
     * @note The operation modifies the current object.
     */
    template <typename BigInt_T>
    void Or(const BigInt_T &bint) noexcept {
        SizeT32 index = (Index() <= bint.Index() ? Index() : bint.Index());

        while (index > 0) {
            Storage()[index] |= bint.Storage()[index];
            --index;
        }

        Storage()[0] |= bint.Storage()[0];
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
     * @brief Performs a bitwise AND operation with another BigInt type.
     *
     * Updates this BigInt in place by applying a bitwise AND between each
     * corresponding storage element of this object and @p bint.
     *
     * Any storage elements beyond the highest valid index of @p bint are cleared,
     * since they cannot contribute to the result. After the operation, leading
     * zero storage elements are removed by adjusting the internal index.
     *
     * @tparam BigInt_T A BigInt.
     * @param bint The value to AND with this BigInt.
     *
     * @note The operation modifies the current object.
     * @note The result is normalized by trimming leading zero storage elements.
     */
    template <typename BigInt_T>
    void And(const BigInt_T &bint) noexcept {
        SizeT32 index = (Index() <= bint.Index() ? Index() : bint.Index());

        while (index_ > index) {
            Storage()[index_] = 0;
            --index_;
        }

        while (index > 0) {
            Storage()[index] &= bint.Storage()[index];
            --index;
        }

        Storage()[0] &= bint.Storage()[0];

        while ((index_ > 0) && (Storage()[index_] == 0)) {
            --index_;
        }
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
     * @brief Adds another BigInt to this BigInt.
     * @param bint The BigInt value to add.
     *
     * Equivalent to calling Add(const BigInt &).
     */
    QENTEM_INLINE void operator+=(const BigInt &bint) noexcept {
        Add(bint);
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
     * @brief Subtracts another BigInt from this BigInt.
     * @param bint The BigInt value to subtract.
     *
     * Equivalent to calling Subtract(const BigInt &).
     */
    QENTEM_INLINE void operator-=(const BigInt &bint) noexcept {
        Subtract(bint);
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
        // Propagate the addition and any carry across limbs
        do {
            const Number_T tmp = storage_[index];
            storage_[index] += number;

            // If no overflow occurred, addition is complete
            if (storage_[index] >= tmp) {
                break;
            }

            ++index;

            if (index > MaxIndex()) {
                index_ = 0;
                index  = 0;
                break;
            }

            // Overflow occurred: set number to 1 to propagate carry
            number = Number_T{1};
        } while (true);

        // update the highest used limb index if necessary
        if ((index > index_) && (number != 0)) {
            index_ = index;
        }
    }

    /**
     * @brief Adds another BigInt to this BigInt.
     * @param bint The BigInt value to add.
     *
     * Performs limb-by-limb addition starting from the least-significant
     * limb. Carry propagation is handled internally by Add(Number_T, SizeT32).
     *
     * The result is stored in this BigInt.
     */
    QENTEM_NOINLINE void Add(const BigInt &bint) noexcept {
        SizeT32 index = 0;

        do {
            Add(bint.Storage()[index], index);
            ++index;
        } while (index <= bint.Index());
    }

    /**
     * @brief Adds another BigInt to this BigInt.
     * @tparam bint The BigInt value to add.
     *
     * Performs limb-by-limb addition starting from the least-significant
     * limb. Carry propagation is handled internally by Add(Number_T, SizeT32).
     *
     * The result is stored in this BigInt.
     */
    template <typename BigInt_T>
    QENTEM_NOINLINE void AddBigInt(const BigInt_T &bint) noexcept {
        SizeT32 index = 0;

        do {
            Add(bint.Storage()[index], index);
            ++index;
        } while ((index <= bint.Index()) && (index <= MaxIndex()));
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
        // Propagate the subtraction and any borrow across limbs
        do {
            const Number_T tmp = storage_[index];
            storage_[index] -= number;

            // If no underflow occurred, subtraction is complete
            if (storage_[index] <= tmp) {
                break;
            }

            if (index == MaxIndex()) {
                index_ = MaxIndex();
                break;
            }

            ++index;

            // Underflow occurred: set number to 1 to propagate borrow
            number = Number_T{1};
        } while (true);

        // If borrow exceeded highest limb, set index_ to the max index

        // Otherwise, trim trailing zero limbs if any
        if (index >= index_) {
            while ((index_ != 0) && (storage_[index_] == 0)) {
                --index_;
            }
        }
    }

    /**
     * @brief Subtracts another BigInt from this BigInt.
     * @param bint The BigInt value to subtract.
     *
     * Performs limb-by-limb subtraction starting from the least-significant
     * limb. Borrow propagation is handled internally by
     * Subtract(Number_T, SizeT32).
     *
     * The result is stored in this BigInt. If the subtraction underflows,
     * the value wraps around within the fixed width of the BigInt.
     */
    QENTEM_NOINLINE void Subtract(const BigInt &bint) noexcept {
        SizeT32 index = 0;

        do {
            Subtract(bint.Storage()[index], index);
            ++index;
        } while (index <= bint.Index());
    }

    /**
     * @brief Subtracts another BigInt from this BigInt.
     * @tparam bint The BigInt value to subtract.
     *
     * Performs limb-by-limb subtraction starting from the least-significant
     * limb. Borrow propagation is handled internally by
     * Subtract(Number_T, SizeT32).
     *
     * The result is stored in this BigInt. If the subtraction underflows,
     * the value wraps around within the fixed width of the BigInt.
     */
    template <typename BigInt_T>
    QENTEM_NOINLINE void SubtractBigInt(const BigInt_T &bint) noexcept {
        SizeT32 index = 0;

        do {
            Subtract(bint.Storage()[index], index);
            ++index;
        } while ((index <= bint.Index()) && (index <= MaxIndex()));
    }

    /**
     * @brief Multiplies this BigInt by a built-in integer value (in place).
     * @tparam multiplier The value to multiply by.
     *
     * Each limb is multiplied by the multiplier, and any overflow is
     * propagated and added to the next higher limb.
     */
    void Multiply(Number_T multiplier) noexcept {
        // Start from the highest limb and propagate carries to higher limbs.
        SizeT32 index = index_;
        ++index;

        do {
            const SizeT32 current_index = index;
            --index;
            // Multiply the current limb; Add() will handle carry propagation.
            // if (storage_[index] != 0)
            Add(DoubleWidthArithmetic<Number_T, BitWidth()>::Multiply(storage_[index], multiplier), current_index);
        } while (index != 0);
    }

    /**
     * @brief Multiplies this BigInt by another BigInt.
     * @tparam result Receives the full multiplication result.
     * @param bint The multiplier.
     *
     * Performs long multiplication by accumulating each limb product at
     * its corresponding result offset:
     *
     *     offset = i + j
     *
     * The smaller operand is used as the outer loop to reduce the number
     * of iterations.
     *
     * The complete product is written to the supplied DoubleBigInt without
     * truncation.
     */
    template <typename BigInt_T>
    void Multiply(BigInt_T &result, const BigInt &bint) const noexcept {
        const Number_T *storage_a;
        const Number_T *storage_b;
        SizeT32         max_index_a;
        SizeT32         max_index_b;

        result.Clear();

        // Use a consistent operand ordering for the multiplication loops.
        if (Index() >= bint.Index()) {
            storage_a   = Storage();
            max_index_a = Index();

            storage_b   = bint.Storage();
            max_index_b = bint.Index();
        } else {
            storage_a   = bint.Storage();
            max_index_a = bint.Index();

            storage_b   = Storage();
            max_index_b = Index();
        }

        SizeT32 index2 = max_index_b;
        SizeT32 offset = max_index_b;
        ++index2;

        // Multiply each limb of the smaller operand by the larger operand
        // and accumulate the partial products into the result at the
        // appropriate limb offsets.
        do {
            SizeT32 index1 = max_index_a;
            ++index1;
            --index2;

            do {
                const SizeT32 current_index = index1;
                --index1;

                Number_T number = storage_a[index1];

                const Number_T carry = DoubleWidthArithmetic<Number_T, BitWidth()>::Multiply(number, storage_b[index2]);

                result.Add(number, (index1 + offset));
                result.Add(carry, (current_index + offset));
            } while (index1 != 0);

            --offset;
        } while (index2 != 0);
    }

    /**
     * @brief Computes the square of this BigInt.
     * @param result Receives the full square result.
     *
     * Computes:
     *
     *     Σ(a[i]² · B^(2i))
     *   + 2·Σ(a[i]a[j] · B^(i+j)), for i > j
     *
     * where B is the limb base (2^BitWidth()).
     *
     * The diagonal terms are accumulated once, while the off-diagonal
     * terms are accumulated twice to account for the symmetry:
     *
     *     a[i]a[j] = a[j]a[i]
     *
     * The result is written to the supplied DoubleBigInt without
     * truncation and is equivalent to Multiply(*this), but avoids
     * redundant multiplications by exploiting the symmetry of squaring.
     */
    template <typename BigInt_T>
    void Square(BigInt_T &result) const noexcept {
        SizeT32 offset = Index();
        ++offset;

        result.Clear();

        do {
            const SizeT32 current_offset = offset;
            --offset;

            Number_T number = storage_[offset];
            Number_T carry  = DoubleWidthArithmetic<Number_T, BitWidth()>::Multiply(number, storage_[offset]);

            SizeT32 sub_offset = (offset << 1U);

            // Add the diagonal term: a[i]² at offset (2 * i).
            result.Add(number, sub_offset);
            result.Add(carry, (current_offset + offset));

            SizeT32 sub_numer_offset = offset;

            while (sub_numer_offset != 0) {
                const SizeT32 current_sub_offset = sub_offset;

                --sub_numer_offset;
                --sub_offset;

                number = storage_[offset];
                carry  = DoubleWidthArithmetic<Number_T, BitWidth()>::Multiply(number, storage_[sub_numer_offset]);

                // Add the off-diagonal term twice:
                // 2 * a[i] * a[j] at offset (i + j).
                result.Add(number, sub_offset);
                result.Add(number, sub_offset);
                result.Add(carry, current_sub_offset);
                result.Add(carry, current_sub_offset);
            }
        } while (offset != 0);
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
        // Start from the highest limb
        const SizeT32  initial_shift = ((BitWidth() - 1U) - Platform::FindLastBit(divisor));
        SizeT32        index         = index_;
        const Number_T tmp           = storage_[index_];

        // Divide the highest limb and get the initial remainder
        storage_[index_]   = (tmp / divisor);
        Number_T remainder = (tmp % divisor);

        // Process all lower limbs, propagating remainder as needed
        while (index != 0) {
            --index;
            DoubleWidthArithmetic<Number_T, BitWidth()>::Divide(remainder, storage_[index], divisor, initial_shift);
        }

        // Update the number of used limbs if highest limb is now zero
        index_ -= static_cast<SizeT32>((index_ != 0) && (storage_[index_] == 0));

        // Return the final remainder
        return remainder;
    }

    /**
     * @brief Divides this BigInt by another BigInt.
     *
     * Computes the integer quotient and remainder of:
     *
     * @code
     * quotient  = (*this) / divisor
     * remainder = (*this) % divisor
     * @endcode
     *
     * The quotient replaces the current value, while the remainder is written
     * to the supplied output parameter.
     *
     * This implementation uses an iterative approximation approach. An initial
     * quotient estimate is generated from the highest limb of the divisor and
     * the most significant portion of the current dividend. The estimate is
     * multiplied by the divisor and compared against the remaining value. The
     * quotient is then refined by alternately adding or subtracting correction
     * terms until the remaining difference becomes smaller than the divisor.
     *
     * A temporary BigInt with additional bit capacity is used during
     * multiplication to prevent intermediate overflow while evaluating
     * quotient estimates.
     *
     * Special cases:
     * - If the dividend is smaller than the divisor, the quotient becomes 0
     *   and the remainder becomes the original dividend.
     * - If the dividend is equal to the divisor, the quotient becomes 1
     *   and the remainder becomes 0.
     * - Otherwise, the quotient and remainder are computed iteratively.
     *
     * @param divisor The value used as the divisor.
     * @param remainder Receives the division remainder.
     *
     * @note The current BigInt is modified and becomes the quotient.
     * @note Division by zero must be prevented by the caller.
     *
     * @post
     * @code
     * original_dividend = (*this * divisor) + remainder
     * remainder < divisor
     * @endcode
     */
    template <typename BigInt_Remainder_T, typename BigInt_Divisor_T>
    void Divide(BigInt_Remainder_T &remainder, const BigInt_Divisor_T &divisor) noexcept {
        if (IsLess(divisor, *this)) {
            BigInt dividend{*this};
            BigInt residual{};

            BigInt_Remainder_T &estimate{remainder};

            BigInt<Number_T, TotalBitWidth() + BitWidth()> product{};

            SizeT32 divisor_highest_bit  = Platform::FindLastBit(divisor.Storage()[divisor.Index()]);
            bool divisor_is_power_of_two = (divisor.Storage()[divisor.Index()] == (Number_T{1} << divisor_highest_bit));
            bool undershot               = true;

            if (divisor_is_power_of_two) {
                residual.SetIndex(divisor.Index());
                residual.Storage()[divisor.Index()] = divisor.Storage()[divisor.Index()];

                divisor_is_power_of_two = IsEqual(residual, divisor);
            }

            residual.Copy(*this);
            Clear();

            while (true) {
                SizeT32 current_highest_bit = estimate.FindLastBit();
                estimate.ShiftRight(divisor.Index() * divisor.BitWidth());

                if (!divisor_is_power_of_two) {
                    estimate.Divide(divisor.Storage()[divisor.Index()]);
                } else {
                    estimate.ShiftRight(divisor_highest_bit);
                }

                estimate.Multiply(product, divisor);

                const SizeT32 product_highest_bit = product.FindLastBit();

                if (product_highest_bit < current_highest_bit) {
                    current_highest_bit -= product_highest_bit;

                    estimate <<= current_highest_bit;
                    product <<= current_highest_bit;
                }

                if (undershot) {
                    Add(estimate);
                } else {
                    Subtract(estimate);
                }

                if (IsGreaterOrEqual(residual, product)) {
                    residual.SubtractBigInt(product);

                    if (IsLess(residual, divisor)) {
                        if (!undershot && residual.IsNotZero()) {
                            *this -= Number_T{1};
                        }

                        break;
                    }

                    estimate.Copy(residual);
                    undershot = true;
                } else {
                    product.SubtractBigInt(residual);

                    if (IsLess(product, divisor)) {
                        if (undershot) {
                            *this -= Number_T{1};
                        }

                        break;
                    }

                    estimate.Copy(product);
                    residual.Copy(product);
                    undershot = !undershot;
                }
            }

            remainder.Copy(dividend);
            Multiply(product, divisor);
            remainder.SubtractBigInt(product);
        } else if (IsEqual(*this, divisor)) {
            remainder.Clear();
            Clear();
            Storage()[0] = Number_T{1};
        } else {
            remainder.Copy(*this);
            Clear();
        }
    }

    /**
     * @brief Computes the Barrett reciprocal for a divisor.
     *
     * Computes:
     *
     *     mu = floor(B^(2k) / divisor)
     *
     * where:
     *
     *     B = 2^(BitWidth())
     *     k = divisor.Index() + 1
     *
     * The reciprocal is used by Barrett reduction to replace expensive
     * division operations with multiplication, shifting, and correction
     * steps during modular arithmetic.
     *
     * @param[out] mu The computed Barrett reciprocal.
     * @param[in] divisor The divisor (modulus) used to compute the reciprocal.
     */
    static void ComputeBarrettMu(BigInt &mu, const BigInt &modulus) noexcept {
        using BiggerBigInt = BigInt<Number_T, (TotalBitWidth() * 2U)>;

        BiggerBigInt remainder{};
        BiggerBigInt numerator{};
        BiggerBigInt expanded_modulus{};

        expanded_modulus.Copy(modulus);

        const SizeT32 index = ((modulus.Index() + 1U) * 2U);

        numerator.SetIndex(index);
        numerator.Storage()[index] = Number_T{1};
        numerator.Divide(remainder, expanded_modulus);

        mu.Copy(numerator);
    }

    /**
     * @brief Reduces the current value modulo a divisor using Barrett reduction.
     *
     * Barrett reduction replaces an expensive division with a precomputed reciprocal
     * and a small number of multiplications, shifts, and subtractions.
     *
     * The reciprocal (`mu`) must be computed beforehand using
     * ComputeBarrettMu():
     *
     * @code
     * BigInt mu{};
     * BigInt::ComputeBarrettMu(mu, modulus);
     *
     * value.ReduceBarrett(mu, modulus);
     * @endcode
     *
     * This function modifies the current value in place:
     *
     * @code
     * this = this % modulus;
     * @endcode
     *
     * The reduction follows the Barrett algorithm:
     *
     * @code
     * q1 = floor(x / b^(k - 1))
     * q2 = q1 * mu
     * q3 = floor(q2 / b^(k + 1))
     * r1 = x mod b^k
     * r2 = (q3 * n) mod b^k
     * r  = r1 - r2
     *
     * if (r < 0) {
     *     r += b^k;
     * }
     *
     * if (r >= n) {
     *     r -= n;
     *     if (r >= n) {
     *         r -= n;
     *     }
     * }
     *
     * @endcode
     *
     * where:
     * - x is the current value.
     * - n is the modulus.
     * - b is the internal limb base (2^BitWidth()).
     * - k is the number of limbs used by the modulus.
     * - mu is the Barrett reciprocal.
     *
     * @param mu Precomputed Barrett reciprocal for the modulus.
     * @param modulus Divisor used for the reduction.
     *
     * @note The input value must be less than b^(2k), where k is the number
     *       of limbs in the modulus and b = 2^BitWidth().
     */
    template <typename BigInt_T>
    void ReduceBarrett(const BigInt_T &modulus, const BigInt_T &mu) noexcept {
        if (IsGreater(*this, modulus)) {
            using BiggerBigInt = BigInt<Number_T, (TotalBitWidth() + BitWidth())>;
            // using BiggerBigInt = BigInt<Number_T, (TotalBitWidth() * 2U)>;

            BigInt        q1{};
            BiggerBigInt  q2{};
            BigInt       &r1{q1};
            BiggerBigInt &r2{q2};
            BigInt        b_k{};

            const SizeT32 k    = (modulus.Index() + 1U);
            const SizeT32 k_p1 = (k + 1U);

            b_k.SetIndex(k_p1);
            b_k.Storage()[k_p1] = Number_T{1};
            b_k.Subtract(Number_T{1});

            // q1.Copy(*this);
            // q1.ShiftRight(modulus.Index() * BitWidth());
            q1.CopyRange(*this, modulus.Index(), ((Index() + 1) - modulus.Index()));

            q1.Multiply(q2, mu);
            q2.ShiftRight((k + 1U) * BitWidth());

            r1.Copy(*this);
            r1.And(b_k);

            Copy(q2); // q2.Multiply(r2, modulus);
            Multiply(q2, modulus);
            // &r2 = q2
            r2.And(b_k);

            if (IsGreaterOrEqual(r1, r2)) {
                r1.SubtractBigInt(r2);
                Copy(r1);
            } else {
                b_k.Clear();
                b_k.SetIndex(k_p1);
                b_k.Storage()[k_p1] = Number_T{1};

                r2.SubtractBigInt(r1);
                b_k.SubtractBigInt(r2);
                Copy(b_k);
            }

            if (IsGreaterOrEqual(*this, modulus)) {
                SubtractBigInt(modulus);

                if (IsGreaterOrEqual(*this, modulus)) {
                    SubtractBigInt(modulus);
                }
            }
        } else if (IsEqual(*this, modulus)) {
            *this = Number_T{0};
        }
    }

    /**
     * @brief Computes modular exponentiation using Barrett reduction.
     *
     * Replaces the current value with:
     *
     *     this = (this^exponent) mod modulus
     *
     * The exponent is processed using the binary square-and-multiply algorithm.
     * All modular reductions are performed using Barrett reduction. A Barrett
     * reciprocal is computed once from the supplied modulus and reused
     * throughout the exponentiation, avoiding repeated division operations.
     *
     * @param exponent Exponent value. A local copy is consumed during bit
     *                 processing.
     * @param modulus Modulus used for reduction operations.
     *
     * @note If exponent is zero, the result is one.
     * @note The current value is treated as the base value.
     * @note Barrett reduction requires:
     *
     *           0 ≤ x < b^(2k)
     *
     *       where b is the limb base and k is the number of limbs in the
     *       modulus.
     */
    void ModExpBarrett(BigInt exponent, const BigInt &modulus) {
        // using BiggerBigInt = BigInt<Number_T, (TotalBitWidth() + BitWidth())>;
        using BiggerBigInt = BigInt<Number_T, (TotalBitWidth() * 2U)>;

        BiggerBigInt base{};

        base.Copy(*this);
        *this = Number_T{1}; // result

        if (exponent.IsNotZero()) {
            BiggerBigInt product{};

            BigInt mu{};
            ComputeBarrettMu(mu, modulus);

            // base = base % modulus;
            base.ReduceBarrett(modulus, mu);

            while (true) {
                if ((exponent.Storage()[0] & Number_T{1}) != 0) {
                    // result = (result * base) % modulus;
                    Multiply(product, base); // product = result * base

                    product.ReduceBarrett(modulus, mu); // result  = product % modulus
                    Copy(product);
                }

                if (exponent != Number_T{1}) {
                    exponent.ShiftRight(1U);

                    // base = (base * base) % modulus;
                    base.Square(product); // product = (base * base)

                    product.ReduceBarrett(modulus, mu); // base = product % modulus
                    base.Copy(product);

                    continue;
                }

                break;
            }
        }
    }

    /**
     * @brief Computes modular exponentiation.
     *
     * Replaces the current value with:
     *
     *     this = (this^exponent) mod modulus
     *
     * The exponent is processed using the binary square-and-multiply algorithm.
     * Intermediate multiplication results are reduced modulo the supplied
     * modulus to prevent unbounded growth. A temporary wider BigInt type is
     * used internally to hold multiplication results prior to reduction.
     *
     * @param exponent Exponent value. A local copy is consumed during bit
     *                 processing.
     * @param modulus Modulus used for reduction operations.
     *
     * @note If exponent is zero, the result is one.
     * @note The current value is treated as the base value.
     */
    void ModExp(BigInt exponent, const BigInt &modulus) noexcept {
        // using BiggerBigInt = BigInt<Number_T, (TotalBitWidth() + BitWidth())>;
        using BiggerBigInt = BigInt<Number_T, (TotalBitWidth() * 2U)>;

        BiggerBigInt base{};

        base.Copy(*this);
        *this = Number_T{1}; // result

        if (exponent.IsNotZero()) {
            BiggerBigInt product{};
            BiggerBigInt remainder{};

            // base = base % modulus;
            base.Divide(remainder, modulus);
            base.Copy(remainder);

            while (true) {
                if ((exponent.Storage()[0] & Number_T{1}) != 0) {
                    // result = (result * base) % modulus;
                    Multiply(product, base);            // product = result * base
                    product.Divide(remainder, modulus); // result  = product % modulus
                    Copy(remainder);
                }

                if (exponent != Number_T{1}) {
                    exponent.ShiftRight(1U);

                    // base = (base * base) % modulus;
                    base.Square(product); // product = (base * base)

                    product.Divide(base, modulus); // base = product % modulus

                    continue;
                }

                break;
            }
        }
    }

    /**
     * @brief Computes modular multiplication.
     *
     * Replaces the current value with:
     *
     *     this = (this * right) mod modulus
     *
     * A temporary wider BigInt type is used internally to hold the
     * multiplication result before modular reduction.
     *
     * @param right Right-hand multiplicand.
     * @param modulus Modulus used for reduction.
     */
    void ModMul(const BigInt &right, const BigInt &modulus) noexcept {
        using BiggerBigInt = BigInt<Number_T, (TotalBitWidth() * 2U)>;
        BiggerBigInt product{};
        BiggerBigInt remainder{};

        Multiply(product, right);

        product.Divide(remainder, modulus);

        Copy(remainder);
    }

    /**
     * @brief Computes modular multiplication using Barrett reduction.
     *
     * Replaces the current value with:
     *
     *     this = (this * right) mod modulus
     *
     * The multiplication is performed using a temporary wider BigInt type
     * to preserve the full product before reduction.
     *
     * @param right Right-hand multiplicand.
     * @param modulus Modulus used for reduction.
     * @param mu Barrett reciprocal computed for the supplied modulus.
     *
     * @note The Barrett reciprocal must be computed from the same modulus.
     * @note Barrett reduction requires:
     *
     *           0 ≤ (this * right) < b^(2k)
     *
     *       where b is the limb base and k is the number of limbs in the
     *       modulus.
     */
    void ModMulBarrett(const BigInt &right, const BigInt &modulus, const BigInt &mu) noexcept {
        using BiggerBigInt = BigInt<Number_T, (TotalBitWidth() * 2U)>;
        BiggerBigInt product{};

        Multiply(product, right);

        product.ReduceBarrett(modulus, mu);

        Copy(product);
    }

    /**
     * @brief Computes the greatest common divisor (GCD) using the Euclidean algorithm.
     *
     * The algorithm repeatedly computes the remainder of the division between
     * the current operands until the right-hand value becomes zero.
     *
     * @param remainder Temporary BigInt used to store intermediate remainders.
     *                  Its contents are overwritten during the computation.
     * @param right Right-hand operand. Its value is consumed and replaced with
     *              intermediate remainders during the computation.
     *
     * @note Upon completion, the current value contains the GCD.
     * @note The value of @p right is modified.
     */
    void GCD(BigInt &remainder, BigInt &right) noexcept {
        // Iterative Euclidean algorithm.

        // temp = a % b;
        // a    = b;
        // b    = temp;

        remainder.Clear();

        while (right.IsNotZero()) {
            Divide(remainder, right);

            Copy(right);
            right.Copy(remainder);
        }
    }

    /**
     * @brief Computes the greatest common divisor (GCD) using the Euclidean algorithm.
     *
     * A temporary remainder value is allocated internally.
     *
     * @param right Right-hand operand. Its value is consumed and replaced with
     *              intermediate remainders during the computation.
     *
     * @note Upon completion, the current value contains the GCD.
     * @note The value of @p right is modified.
     */
    void GCD(BigInt &right) noexcept {
        BigInt remainder{};

        GCD(remainder, right);
    }

    /**
     * @brief Computes the least common multiple (LCM).
     *
     * The result is computed using:
     *
     *     LCM(a, b) = (a * b) / GCD(a, b)
     *
     * A temporary wider BigInt type is used internally to preserve the full
     * multiplication result before division.
     *
     * @param right Right-hand operand. Its value is consumed during the GCD
     *              computation and is modified.
     *
     * @note If the current value is zero, the result remains zero.
     * @note The value of @p right is modified.
     */
    void LCM(BigInt &right) noexcept {
        using BiggerBigInt = BigInt<Number_T, (TotalBitWidth() * 2U)>;
        BiggerBigInt product{};
        BigInt       remainder{};

        if (IsNotZero()) {
            // (a * b) / GCD(a, b).
            Multiply(product, right);

            GCD(remainder, right);

            remainder.Clear();
            product.Divide(remainder, *this);

            Copy(product);
        }
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
            while ((storage_[index] == 0) && (index != 0)) {
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
            if (index_ != MaxIndex() && (carry != 0)) {
                ++index_;
                storage_[index_] = 0;
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
     * @brief Finds the index of the least significant set bit.
     * @return The bit position of the least significant set bit (starting from 0).
     *
     * @note This function requires the value to be non-zero.
     */
    QENTEM_INLINE SizeT32 FindFirstBit() const noexcept {
        SizeT32 index = 0;

        // Skip zero limbs from the least significant end
        while ((storage_[index] == 0) && (index < index_)) {
            ++index;
        }

        // Use a platform-specific method to find the least significant set bit
        // within the first non-zero limb
        return (Platform::FindFirstBit(storage_[index]) + (index * BitWidth()));
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

    /**
     * @brief Normalizes the BigInt by updating the highest active limb index.
     *
     * Removes leading zero limbs from the most significant end and ensures
     * index_ points to the highest non-zero limb. A zero value is represented
     * with index_ equal to 0.
     */
    QENTEM_INLINE void Normalize() noexcept {
        while ((storage_[index_] == 0) && (index_ > 0)) {
            --index_;
        }
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
     * @return Storage capacity in bits, rounded up to a full limb.
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
    QENTEM_INLINE bool IsMultiLimb() const noexcept {
        return (index_ != 0);
    }

    /**
     * @brief Checks if this BigInt value is zero.
     * @return True if value is zero.
     */
    QENTEM_INLINE bool IsZero() const noexcept {
        return (*this == 0);
    }

    /**
     * @brief Checks if this BigInt is not zero.
     * @return True if value is not zero.
     */
    QENTEM_INLINE bool IsNotZero() const noexcept {
        return (*this != 0);
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

    /**
     * @brief Copies the value from another BigInt.
     * @tparam src The source BigInt to copy from.
     *
     * Copies all active limbs and zeros out any unused higher limbs.
     */
    template <typename BigInt_T>
    void Copy(const BigInt_T &src) noexcept {
        SizeT32 index = 0;

        // Zero out any extra limbs from previous state
        while (index_ > src.Index()) {
            storage_[index_] = 0;
            --index_;
        }

        // Copy each used limb from source
        while ((index <= src.Index()) && (index <= MaxIndex())) {
            storage_[index] = src.Storage()[index];
            ++index;
        }

        index_ = index;
        --index_;
    }

    /**
     * @brief Copies a range of limbs from a source BigInt.
     *
     * Copies @p count limbs starting at @p index from the source BigInt and
     * stores them starting at limb index 0 in the destination BigInt.
     *
     * Example:
     *
     * Source:
     *     [11, 22, 33, 44, 55]
     *
     * CopyRange(src, 2, 3):
     *     [33, 44, 55]
     *
     * @tparam BigInt_T Source BigInt type.
     * @param[in] src Source BigInt.
     * @param[in] index Starting limb index.
     * @param[in] count Number of limbs to copy.
     */
    template <typename BigInt_T>
    void CopyRange(const BigInt_T &src, SizeT32 index, const SizeT32 count) noexcept {
        const SizeT32 end_index = (count + index);
        SizeT32       offset    = 0;

        if (index <= src.Index()) {
            while ((index < end_index) && (offset <= MaxIndex())) {
                storage_[offset] = src.Storage()[index];
                ++offset;
                ++index;
            }
        }

        const SizeT32 new_index = ((offset != 0) ? (offset - 1) : 0);

        while (offset <= index_) {
            storage_[offset] = 0;
            ++offset;
        }

        index_ = new_index;
    }

  protected:
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
    void doOperation(N_Number_T number) noexcept {
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
    Number_T storage_[MaxIndex() + Number_T{1}];

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
        SizeT16 dividend16{dividend_high}; // Pack high byte
        dividend16 <<= shift_;             // Shift up by 8 bits
        dividend16 |= dividend_low;        // Pack in low byte

        const SizeT16 tmp = dividend16;

        dividend16    = static_cast<Number_T>(tmp / divisor); // New quotient in low
        dividend_high = static_cast<Number_T>(tmp % divisor); // New remainder in high
        dividend_low  = static_cast<Number_T>(dividend16);
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
        SizeT32 dividend32{dividend_high}; // Pack high 16 bits
        dividend32 <<= shift_;             // Shift to high word
        dividend32 |= dividend_low;        // Add in low 16 bits

        const SizeT32 tmp = dividend32;

        dividend32    = static_cast<Number_T>(tmp / divisor); // New quotient in low
        dividend_high = static_cast<Number_T>(tmp % divisor); // New remainder in high
        dividend_low  = static_cast<Number_T>(dividend32);
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
                                     const SizeT32 initial_shift) noexcept {
        if constexpr (!QentemConfig::Is64bit) {
            DoubleWidthArithmetic<Number_T, 64>::Divide(dividend_high, dividend_low, divisor, initial_shift);
            return;
        } else {
            SizeT64 dividend64{dividend_high}; // Pack high 32 bits
            dividend64 <<= shift_;             // Shift to high dword
            dividend64 |= dividend_low;        // Add in low 32 bits

            const SizeT64 tmp = dividend64;

            dividend64    = static_cast<Number_T>(tmp / divisor); // New quotient in low
            dividend_high = static_cast<Number_T>(tmp % divisor); // New remainder in high
            dividend_low  = static_cast<Number_T>(dividend64);
        }
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
        if constexpr (!QentemConfig::Is64bit) {
            return DoubleWidthArithmetic<Number_T, 64>::Multiply(number, multiplier);
        } else {
            SizeT64 number64{number};            // Promote to 64 bits
            number64 *= multiplier;              // 64-bit multiplication
            number = Number_T(number64);         // Store low 32 bits
            return Number_T(number64 >> shift_); // Return high 32 bits as carry
        }
    }

  private:
    static constexpr SizeT32 shift_ = 32U; ///< Number of bits per limb.
};
////////////////////////////////////////////////////
/**
 * @brief Double-width arithmetic for 64-bit limbs (128-bit intermediate math).
 *
 * This specialization is used by BigInt for division and multiplication when each limb
 * is 64 bits wide (e.g., `uint64_t`). It implements core arithmetic operations on a
 * two-limb value, allowing full-precision multi-limb math on platforms that do not
 * provide native 128-bit arithmetic.
 *
 * The division implementation uses divisor normalization, quotient estimation,
 * and correction techniques to perform 128-bit by 64-bit division using only
 * native-width operations. The multiplication implementation computes the full
 * 128-bit product by splitting operands into half-width components and combining
 * the partial results.
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
     * This function is used by BigInt long-division routines when a double-width dividend
     * must be divided using only native-width arithmetic.
     *
     * @param[in,out] dividend_high The upper half of the dividend and resulting remainder.
     * @param[in,out] dividend_low  The lower half of the dividend and resulting quotient.
     * @param divisor The divisor.
     * @param initial_shift Normalization shift used to align the divisor's highest set bit.
     *
     * Algorithm overview:
     *   1. Normalize the divisor and dividend.
     *   2. Estimate and correct quotient digits.
     *   3. Accumulate the quotient into the lower half.
     *   4. Restore the remainder to its original scale.
     *   5. Handle carry and overflow corrections.
     */
    static void Divide(Number_T &dividend_high, Number_T &dividend_low, const Number_T divisor,
                       const SizeT32 initial_shift) noexcept {
        constexpr Number_T OVERFLOW_DIVIDEND = (Number_T{1} << (width_ - 1U));

        // Preserve the remainder from the initial low-limb division.
        Number_T tmp         = dividend_low;
        dividend_low         = (tmp / divisor);
        const Number_T carry = (tmp % divisor);

        // Normalize the divisor so its highest bit is set.
        const Number_T divisor_shifted = (divisor << initial_shift);

        // Split the normalized divisor into most- and least-significant halves.
        const Number_T divisor_low  = (divisor_shifted >> shift_); // Most significant half
        const Number_T divisor_high = (divisor_shifted & mask_);   // Least significant half

        // Normalize the upper dividend limb.
        dividend_high <<= initial_shift;

        // Estimate the first quotient digit using the most-significant half
        // of the normalized divisor.
        Number_T quotient = (dividend_high / divisor_low);
        dividend_high %= divisor_low;
        Number_T remainder = (quotient * divisor_high);

        dividend_high <<= shift_;

        // Correct an overestimated quotient digit.
        if (dividend_high < remainder) {
            --quotient;

            if ((remainder - dividend_high) > divisor_shifted) {
                --quotient;
                remainder -= divisor_shifted;
            }

            remainder -= divisor_shifted;
        }

        // Number_T tmp = static_cast<Number_T>(dividend_high < remainder);
        // quotient -= tmp;

        // Number_T tmp2 = (static_cast<Number_T>((remainder - dividend_high) > divisor_shifted) & tmp);
        // quotient -= tmp2;
        // remainder -= (divisor_shifted & (static_cast<Number_T>(0) - tmp2));
        // remainder -= (divisor_shifted & (static_cast<Number_T>(0) - tmp));

        dividend_high -= remainder;

        // Store the first quotient digit.
        quotient <<= shift_;
        dividend_low += quotient;

        // Estimate the second quotient digit.
        tmp           = dividend_high;
        quotient      = (tmp / divisor_low);
        dividend_high = (tmp % divisor_low);
        remainder     = (quotient * divisor_high);

        dividend_high <<= shift_;

        // Correct an overestimated quotient digit.
        if (dividend_high < remainder) {
            --quotient;

            if ((remainder - dividend_high) > divisor_shifted) {
                --quotient;
                remainder -= divisor_shifted;
            }

            remainder -= divisor_shifted;
        }

        // tmp = static_cast<Number_T>(dividend_high < remainder);
        // quotient -= tmp;

        // tmp2 = (static_cast<Number_T>((remainder - dividend_high) > divisor_shifted) & tmp);
        // quotient -= tmp2;
        // remainder -= (divisor_shifted & (static_cast<Number_T>(0) - tmp2));
        // remainder -= (divisor_shifted & (static_cast<Number_T>(0) - tmp));

        dividend_high -= remainder;

        // Store the second quotient digit.
        dividend_low += quotient;

        // Restore the remainder to its original scale.
        dividend_high >>= initial_shift;

        // Reintegrate the remainder from the initial low-limb division.
        const Number_T original_dividend_high = dividend_high;
        dividend_high += carry;

        // If adding the carried remainder overflows, adjust both the
        // remainder and quotient accordingly.
        if (original_dividend_high > dividend_high) {
            dividend_high += ((OVERFLOW_DIVIDEND % (divisor >> 1U)) << 1U);
            ++dividend_low;
        }

        // tmp = static_cast<Number_T>(original_dividend_high > dividend_high);

        // dividend_high += (((OVERFLOW_DIVIDEND % (divisor >> 1U)) << 1U) & (static_cast<Number_T>(0) - tmp));
        // dividend_low += tmp;

        // Final correction if the remainder is still larger than the divisor.
        if (dividend_high >= divisor) {
            dividend_high -= divisor;
            ++dividend_low;
        }

        // tmp = static_cast<Number_T>(dividend_high >= divisor);
        // dividend_high -= (divisor & (static_cast<Number_T>(0) - tmp));
        // dividend_low += tmp;
    }

    /**
     * @brief Performs 128-bit multiplication using two 64-bit operands.
     *
     * Multiplies @p number by @p multiplier, storing the lower 64 bits of the
     * result in @p number and returning the upper 64 bits.
     *
     * The operands are split into half-width components and multiplied as partial
     * products. The partial results are then combined to reconstruct the full
     * 128-bit product using only native-width arithmetic.
     *
     * @param[in,out] number The left operand. Replaced with the lower 64 bits of the result.
     * @param multiplier The right operand.
     * @return The upper 64 bits of the result.
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
