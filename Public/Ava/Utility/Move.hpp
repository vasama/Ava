#pragma once

#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Misc.hpp"

namespace Ava {

template<typename T>
constexpr Ava_FORCEINLINE RemoveRef<T>&& Move(T&& ref)
{
	return static_cast<RemoveRef<T>&&>(ref);
}

} // namespace Ava
