#pragma once

#include "Ava/Meta/Condition.hpp"

#include <type_traits>

namespace Ava {

template<typename T>
using Decay = std::decay_t<T>;

template<typename T1, typename T2>
static constexpr bool IsSame = std::is_same_v<T1, T2>;

template<typename TBase, typename T>
static constexpr bool IsBaseOf = std::is_base_of_v<TBase, T>;

template<typename T>
static constexpr bool IsEmpty = std::is_empty_v<T>;

template<typename T>
static constexpr bool IsConst = std::is_const_v<T>;

template<typename T>
using RemoveConst = std::remove_const_t<T>;

template<typename T, typename TSource>
using CopyConst = Condition<IsConst<TSource>, const T, T>;

template<typename T>
static constexpr bool IsRef = std::is_reference_v<T>;

template<typename T>
static constexpr bool IsLRef = std::is_lvalue_reference_v<T>;

template<typename T>
static constexpr bool IsRRef = std::is_rvalue_reference_v<T>;

template<typename T>
using RemoveRef = std::remove_reference_t<T>;

template<typename T, typename TSource>
using CopyRef = Condition<IsRRef<TSource>, T&&, T&>;

template<typename T>
static constexpr bool IsPointer = std::is_pointer_v<T>;

template<typename T>
static constexpr bool IsInteger = std::is_integral_v<T>;

template<typename T>
static constexpr bool IsSigned = std::is_signed_v<T>;

template<typename T>
static constexpr bool IsUnsigned = std::is_unsigned_v<T>;

template<typename TFrom, typename T>
static constexpr bool HasConv = std::is_convertible_v<TFrom, T>;

} // namespace Ava
