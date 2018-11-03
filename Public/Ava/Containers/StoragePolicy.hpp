#pragma once

#include "Ava/Types.hpp"

namespace Ava::StoragePolicy {

template<iword TCapacity>
struct Local
{
	static_assert(TCapacity > 0);
	static constexpr iword Capacity = TCapacity;
};

template<typename TAllocator>
struct Remote
{
	typedef TAllocator AllocatorType;
};

template<typename TAllocator, iword TCapacity>
struct Small
{
	typedef TAllocator AllocatorType;

	static_assert(TCapacity > 0);
	static constexpr iword Capacity = TCapacity;
};

} // namespace Ava::StoragePolicy
