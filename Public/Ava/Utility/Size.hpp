#pragma once

#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"

namespace Ava {

template<typename T, uword TSize>
constexpr Ava_FORCEINLINE iword Size(const T(&)[TSize])
{
	return (iword)TSize;
}

template<typename T>
Ava_FORCEINLINE auto Size(const T& container)
	-> decltype((iword)Ava_Ext_Size(container))
{
	return (iword)Ava_Ext_Size(container);
}

template<typename T>
Ava_FORCEINLINE auto Size(const T& container)
	-> decltype((iword)container.Size())
{
	return (iword)container.Size();
}

} // namespace Ava
