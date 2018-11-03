#pragma once

#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"

#include <math.h>

#include "Ava/Private/Platform/Math.hpp"

namespace Ava::Math {

template<typename T>
constexpr Ava_FORCEINLINE T Min(T a, T b)
{
	return a < b ? a : b;
}

template<typename T>
constexpr Ava_FORCEINLINE T Max(T a, T b)
{
	return a < b ? b : a;
}

#include "Ava/Private/Platform/Math_Math.ipp"

} // namespace Ava::Math
