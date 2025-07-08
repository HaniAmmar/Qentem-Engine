/**
 * @file QTraits.hpp
 * @brief Internal type traits for Qentem Engine containers.
 *
 * This header defines compile-time type traits and helpers used internally by
 * Qentem containers and adapters. Notably, it provides the IsNumber<Type_T> trait,
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
namespace QTraits {

template <typename Type_T>
struct RemoveCV {
    using Type = Type_T;
};

template <typename Type_T>
struct RemoveCV<const Type_T> {
    using Type = Type_T;
};

template <typename Type_T>
struct RemoveCV<volatile Type_T> {
    using Type = Type_T;
};

template <typename Type_T>
struct RemoveCV<const volatile Type_T> {
    using Type = Type_T;
};

template <typename Type_T>
struct RemoveReference {
    using Type = Type_T;
};

template <typename Type_T>
struct RemoveReference<Type_T &> {
    using Type = Type_T;
};

template <typename Type_T>
struct RemoveReference<Type_T &&> {
    using Type = Type_T;
};

template <typename Type_T>
struct RemovePointer {
    using Type = Type_T;
};

template <typename Type_T>
struct RemovePointer<Type_T *> {
    using Type = Type_T;
};

template <typename Type_T>
struct RemoveExtent {
    using Type = Type_T;
};

template <typename Type_T, unsigned int N>
struct RemoveExtent<Type_T[N]> {
    using Type = Type_T;
};

template <typename Type_T>
struct RemoveExtent<Type_T[]> {
    using Type = Type_T;
};

template <typename Type_T>
struct Decay {
    using Type = typename RemoveCV<
        typename RemoveReference<typename RemovePointer<typename RemoveExtent<Type_T>::Type>::Type>::Type>::Type;
};

template <typename Type_T>
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

} // namespace QTraits
} // namespace Qentem

#endif
