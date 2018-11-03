#pragma once

#include "Ava/Memory/Allocator.hpp"
#include "Ava/Utility/Move.hpp"

namespace Ava {

template<typename TAllocator>
class AllocatorDeleter : TAllocator
{
public:
	AllocatorDeleter() = default;

	AllocatorDeleter(TAllocator alloc)
		: TAllocator(Move(alloc))
	{
	}

	template<typename T>
	void operator()(T* object)
	{
		Delete(static_cast<const TAllocator&>(*this), object);
	}
};

} // namespace Ava
