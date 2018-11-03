#pragma once

#include "Ava/Memory/Allocator.hpp"
#include "Ava/Debug/Assert.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"

namespace Ava {

template<typename TResource>
class ResourceAllocator : public Allocator<ResourceAllocator<TResource>>
{
public:
	Ava_FORCEINLINE ResourceAllocator()
		: ResourceAllocator(TResource::GetDefaultResource())
	{
	}

	explicit Ava_FORCEINLINE ResourceAllocator(TResource* resource)
		: m_resource(resource)
	{
		Ava_Assert(resource);
	}

	Ava_FORCEINLINE void* Allocate(uword size) const
	{
		return m_resource->Allocate(size);
	}

	Ava_FORCEINLINE void Deallocate(void* block, uword size) const
	{
		m_resource->Deallocate(block, size);
	}

	Ava_FORCEINLINE TResource* GetResource() const
	{
		return m_resource;
	}

private:
	TResource* m_resource;
};

template<typename TResource>
Ava_FORCEINLINE bool operator==(const ResourceAllocator<TResource>& a, const ResourceAllocator<TResource>& b)
{
	TResource* ar = a.GetResource();
	TResource* br = b.GetResource();
	return ar == br || ar->Equals(br);
}

template<typename TResource>
Ava_FORCEINLINE bool operator!=(const ResourceAllocator<TResource>& a, const ResourceAllocator<TResource>& b)
{
	TResource* ar = a.GetResource();
	TResource* br = b.GetResource();
	return ar != br && !ar->Equals(br);
}

} // namespace Ava
