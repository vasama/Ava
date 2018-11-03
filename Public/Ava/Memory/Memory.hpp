#pragma once

#include "Ava/Debug/Assert.hpp"
#include "Ava/Math/Bits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Private/Export.hpp"
#include "Ava/Types.hpp"

#include "Ava/Private/Platform/Memory.hpp"

namespace Ava::Memory {

#include "Ava/Private/Platform/Memory.ipp"

Ava_API void* ZeroSecure(void* dst, uword size);

namespace Constexpr {

constexpr uword CalculateAlignment(uword x)
{
	if (x == 0)
		return 1;

	if (Math::Constexpr::IsPowerOfTwo(x))
		return x;

	return Math::Constexpr::NextPowerOfTwo(x);
}

} // namespace Constexpr

} // namespace Ava::Memory
