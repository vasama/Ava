#pragma once

#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"

namespace Ava {

template<typename T, uword TSize>
Ava_FORCEINLINE T* Begin(T(&array)[TSize])
{
	return array;
}

template<typename T, uword TSize>
Ava_FORCEINLINE T* End(T(&array)[TSize])
{
	return array + TSize;
}

template<typename T>
Ava_FORCEINLINE auto Begin(T&& range)
	-> decltype(begin(Forward<T>(range)))
{
	return begin(Forward<T>(range));
}

template<typename T>
Ava_FORCEINLINE auto End(T&& range)
	-> decltype(end(Forward<T>(range)))
{
	return end(Forward<T>(range));
}

} // namespace Ava
