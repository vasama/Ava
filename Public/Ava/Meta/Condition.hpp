#pragma once

namespace Ava {

namespace Private::Meta {

template<bool>
struct Condition;

template<>
struct Condition<0>
{
	template<typename, typename T>
	using Select = T;
};

template<>
struct Condition<1>
{
	template<typename T, typename>
	using Select = T;
};

} // namespace Private::Meta

template<bool TBool, typename T1, typename T0>
using Condition = typename Private::Meta
	::Condition<TBool>::template Select<T1, T0>;

} // namespace Ava
