#pragma once

#include "Ava/Debug/Assert.hpp"
#include "Ava/Memory/Allocator.hpp"

#include <stdlib.h>

namespace Ava {

struct CAllocator : Allocator<CAllocator>
{
	Ava_FORCEINLINE void* Allocate(uword size) const
	{
		void* block = ::malloc(size);
		Ava_AssertSlow(block);
		return block;
	}

	Ava_FORCEINLINE void Deallocate(void* block, uword) const
	{
		free(block);
	}
};

constexpr Ava_FORCEINLINE bool operator==(const CAllocator&, const CAllocator&)
{
	return true;
}

constexpr Ava_FORCEINLINE bool operator!=(const CAllocator&, const CAllocator&)
{
	return false;
}

} // namespace Ava
