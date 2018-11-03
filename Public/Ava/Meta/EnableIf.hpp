#pragma once

namespace Ava {

namespace Private::Meta {

template<bool>
struct EnableIf;

template<>
struct EnableIf<0>
{
};

template<>
struct EnableIf<1>
{
	template<typename T>
	using Select = T;
};

} // namespace Private::Meta

template<bool TBool, typename T = void>
using EnableIf = typename Private::Meta::EnableIf<TBool>::template Select<T>;

} // namespace Ava
