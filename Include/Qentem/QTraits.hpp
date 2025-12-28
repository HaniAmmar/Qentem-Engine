/**
 * @file QTraits.hpp
 * @brief Core compile-time type traits for Qentem Engine.
 *
 * This header defines a collection of essential template metaprogramming tools used
 * throughout Qentem for type manipulation, function inspection, and SFINAE logic.
 *
 * It provides foundational type transformations such as RemoveCV, RemovePointer, and Decay,
 * type detection like IsNumber<Type_T>, and callable introspection traits including
 * FunctionParentType and FunctionFirstArgType.
 *
 * @author Hani Ammar
 * @date 2025
 * @copyright MIT License
 */

#ifndef QENTEM_TRAITS_H
#define QENTEM_TRAITS_H

namespace Qentem {
namespace QTraits {
///////////////////////////////////////////////////////////////
//                    Type Transformations                   //
///////////////////////////////////////////////////////////////
template <typename Type_T>
struct ReferenceType {
    using Type = Type_T;
};

template <typename Type_T>
struct ReferenceType<Type_T &> {
    using Type = Type_T;
};

template <typename Type_T>
struct ReferenceType<Type_T &&> {
    using Type = Type_T;
};

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

///////////////////////////////////////////////////////////////
//                    Type Categorization                    //
///////////////////////////////////////////////////////////////
template <typename>
struct IsLValueReference {
    static constexpr bool Value{false};
};

template <typename Type_T>
struct IsLValueReference<Type_T &> {
    static constexpr bool Value{true};
};

template <typename>
struct IsRValueReference {
    static constexpr bool Value{false};
};

template <typename Type_T>
struct IsRValueReference<Type_T &&> {
    static constexpr bool Value{true};
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

///////////////////////////////////////////////////////////////
//                   Function Traits                         //
///////////////////////////////////////////////////////////////

/**
 * @brief Trait to extract parent class and return type from a member function pointer.
 *
 * For non-static member function pointers, HasParent is true, and ClassType/ReturnType are provided.
 * For static member functions or free functions, HasParent is false, and ClassType/ReturnType are not defined.
 */
template <typename>
struct FunctionParentType_T {
    static constexpr bool HasParent = false;
};

// Non-const member function pointer
template <typename Class_T, typename Return_T, typename... MethodArgs_T>
struct FunctionParentType_T<Return_T (Class_T::*)(MethodArgs_T...)> {
    static constexpr bool HasParent = true;
    using ClassType                 = Class_T;
    using ReturnType                = Return_T;
};

// Const member function pointer
template <typename Class_T, typename Return_T, typename... MethodArgs_T>
struct FunctionParentType_T<Return_T (Class_T::*)(MethodArgs_T...) const> {
    static constexpr bool HasParent = true;
    using ClassType                 = Class_T;
    using ReturnType                = Return_T;
};

/**
 * @brief Trait to extract the first argument type from a parameter pack.
 *
 * Causes a compile error if called with an empty argument list.
 */
template <typename... Args_T>
struct FunctionFirstArgType_T;

template <typename FirstArg_T, typename... Rest_T>
struct FunctionFirstArgType_T<FirstArg_T, Rest_T...> {
    using Type = FirstArg_T;
};

/**
 * @brief Convenience aliases for extracting traits directly.
 */
template <typename Type_T>
using FunctionParentType = typename FunctionParentType_T<Type_T>::ClassType;

template <typename Type_T>
using FunctionReturnType = typename FunctionParentType_T<Type_T>::ReturnType;

template <typename... Args_T>
using FunctionFirstArgType = typename FunctionFirstArgType_T<Args_T...>::Type;

} // namespace QTraits
} // namespace Qentem

#endif // QENTEM_QTRAITS_H
