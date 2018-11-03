#pragma once

#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Misc.hpp"

namespace Ava {

template<typename T>
constexpr Ava_FORCEINLINE T&& Forward(RemoveRef<T>& ref)
{
	return static_cast<T&&>(ref);
}

template<typename T>
constexpr Ava_FORCEINLINE T&& Forward(RemoveRef<T>&& ref)
{
	return static_cast<T&&>(ref);
}

} // namespace Ava
