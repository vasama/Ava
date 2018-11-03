#pragma once

namespace Ava {

template<typename T, T TValue>
struct BasicConstant
{
	typedef decltype(TValue) Type;
	static constexpr Type Value = TValue;
};

template<auto TValue, typename T = decltype(TValue)>
using Constant = BasicConstant<T, TValue>;

template<bool TValue>
using BoolConstant = BasicConstant<bool, TValue>;

typedef BoolConstant<0> False;
typedef BoolConstant<1> True;

} // namespace Ava
