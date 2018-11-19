#pragma once

namespace Ava {

namespace Private::Meta {

template<typename T, typename...>
struct Identity
{
	typedef T Type;
};

} // namespace Private::Meta

template<typename T, typename... Ts>
using Identity = typename Private::Meta::Identity<T, Ts...>::Type;

} // namespace Ava
