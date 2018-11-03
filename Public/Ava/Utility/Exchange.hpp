#pragma once

#include "Ava/Misc.hpp"
#include "Ava/Utility/Move.hpp"

namespace Ava {

template<typename T, typename TIn = T>
Ava_FORCEINLINE T Exchange(T& object, TIn value)
{
	T result = Move(object);
	object = Move(value);
	return result;
}

} // namespace Ava
