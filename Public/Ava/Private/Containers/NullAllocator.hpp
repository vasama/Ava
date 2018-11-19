#pragma once

#include "Ava/Misc.hpp"

namespace Ava::Private::Containers {

struct NullAllocator
{
};

constexpr Ava_FORCEINLINE bool operator==(
	const NullAllocator&, const NullAllocator&)
{
	return true;
}

constexpr Ava_FORCEINLINE bool operator!=(
	const NullAllocator&, const NullAllocator&)
{
	return false;
}

} // namespace Ava::Private::Containers
