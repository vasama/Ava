#pragma once

#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"

namespace Ava {

template<typename TAllocator>
class AllocatorResource : TAllocator
{
public:
	Ava_FORCEINLINE AllocatorResource()
		: TAllocator()
	{
	}

	Ava_FORCEINLINE AllocatorResource(const TAllocator& alloc)
		: TAllocator(alloc)
	{
	}

	Ava_FORCEINLINE AllocatorResource& operator=(const TAllocator& alloc)
	{
		TAllocator::operator=(alloc);
		return *this;
	}

	Ava_FORCEINLINE void* Allocate(uword size)
	{
		return TAllocator::Allocate(size);
	}

	Ava_FORCEINLINE void Deallocate(void* block, uword size)
	{
		TAllocator::Deallocate(block, size);
	}

	Ava_FORCEINLINE const TAllocator& GetAllocator() const
	{
		return static_cast<const TAllocator&>(*this);
	}

	template<typename T>
	friend Ava_FORCEINLINE bool operator==(
		const AllocatorResource<T>& lhs, const AllocatorResource<T>& rhs)
	{
		return static_cast<const T&>(lhs) == static_cast<const T&>(rhs);
	}

	template<typename T>
	friend Ava_FORCEINLINE bool operator!=(
		const AllocatorResource<T>& lhs, const AllocatorResource<T>& rhs)
	{
		return static_cast<const T&>(lhs) != static_cast<const T&>(rhs);
	}
};

} // namespace Ava
