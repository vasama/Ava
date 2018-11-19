#pragma once

namespace Ava {

namespace Private::Meta_ {

template<typename T, typename...>
struct Identity
{
	typedef T Type;
};

} // namespace Private::Meta_

template<typename T, typename... Ts>
using Identity = typename Private::Meta_::Identity<T, Ts...>::Type;

} // namespace Ava
