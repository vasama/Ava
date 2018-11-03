#pragma once

#include "Ava/Meta/Constant.hpp"

namespace Ava {

namespace Private::Meta_Detect {

struct None;

struct Result_0 : BoolConstant<0>
{
	typedef None Type;

	template<typename TDefault>
	using Default = TDefault;
};

template<typename T>
struct Result_1 : BoolConstant<1>
{
	typedef T Type;

	template<typename>
	using Default = T;
};

template<template<typename...> typename T, typename... TArgs>
auto Detect(int) -> decltype(Result_1<T<TArgs...>>());

template<template<typename...> typename T, typename... TArgs>
auto Detect(...) -> Result_0;

} // namespace Private::Meta_Detect

template<template<typename...> typename T, typename... TArgs>
using Detect = decltype(Private::Meta_Detect::Detect<T, TArgs...>(0));

} // namespace Ava
