#pragma once

#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Types.hpp"

namespace Ava {

namespace Ext {

template<typename T>
struct SelectKeyClass
{
	typedef T Type;
};

template<typename T, uword TSize>
struct SelectKeyClass<T[TSize]>
{
	typedef T Type[1];
};

} // namespace Ext

template<typename T>
using SelectKeyClass = typename Ext::SelectKeyClass<T>::Type;

template<typename T1, typename T2>
constexpr bool IsSameKeyClass = IsSame<SelectKeyClass<T1>, SelectKeyClass<T2>>;

} // namespace Ava
