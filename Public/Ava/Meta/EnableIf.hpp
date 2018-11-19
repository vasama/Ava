#pragma once

namespace Ava {

namespace Private::Meta_ {

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

} // namespace Private::Meta_

template<bool TBool, typename T = void>
using EnableIf = typename Private::Meta_::EnableIf<TBool>::template Select<T>;

} // namespace Ava
