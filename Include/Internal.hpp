/**
 * @file Internal.hpp
 * @brief Internal type traits for Qentem Engine containers.
 *
 * This header defines compile-time type traits and helpers used internally by
 * Qentem containers and adapters. Notably, it provides the IsNumber<T> trait,
 * which can be used in template metaprogramming to detect and specialize
 * behavior for numeric types (integral and floating-point).
 *
 * These internal utilities are designed to support template-based selection,
 * SFINAE, and adapter logic throughout Qentem's hash table and container code.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_INTERNAL_H
#define QENTEM_INTERNAL_H

namespace Qentem {
namespace Internal {

template <typename T>
struct IsNumber {
    static constexpr bool value = false;
};

template <>
struct IsNumber<char> {
    static constexpr bool value = true;
};

template <>
struct IsNumber<unsigned char> {
    static constexpr bool value = true;
};

template <>
struct IsNumber<short> {
    static constexpr bool value = true;
};

template <>
struct IsNumber<unsigned short> {
    static constexpr bool value = true;
};

template <>
struct IsNumber<int> {
    static constexpr bool value = true;
};

template <>
struct IsNumber<unsigned int> {
    static constexpr bool value = true;
};

template <>
struct IsNumber<long> {
    static constexpr bool value = true;
};

template <>
struct IsNumber<unsigned long> {
    static constexpr bool value = true;
};

template <>
struct IsNumber<long long> {
    static constexpr bool value = true;
};

template <>
struct IsNumber<unsigned long long> {
    static constexpr bool value = true;
};

template <>
struct IsNumber<float> {
    static constexpr bool value = true;
};

template <>
struct IsNumber<double> {
    static constexpr bool value = true;
};

} // namespace Internal
} // namespace Qentem

#endif
