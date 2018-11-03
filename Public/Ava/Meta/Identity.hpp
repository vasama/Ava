#pragma once

namespace Ava {

namespace Private::Meta {

template<typename...>
struct Identity
{
	template<typename T>
	using Select = T;
};

} // namespace Private::Meta

template<typename T, typename... Ts>
using Identity = typename Private::Meta::Identity<Ts...>::template Select<T>;

} // namespace Ava
