#pragma once

#include "Ava/Memory/VirtualResource.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Private/Ebo.hpp"
#include "Ava/Types.hpp"
#include "Ava/Utility/Forward.hpp"
#include "Ava/Utility/Move.hpp"

namespace Ava {

template<typename TResource>
struct PolymorphicResource : Ava_EBO(VirtualResource, TResource)
{
	Ava_FORCEINLINE PolymorphicResource()
		: VirtualResource(this)
	{
	}

	template<typename... TArgs>
	Ava_FORCEINLINE PolymorphicResource(TArgs&&... args)
		: VirtualResource(this), TResource(Forward<TArgs>(args)...)
	{
	}

	Ava_FORCEINLINE PolymorphicResource& operator=(TResource&& resource)
	{
		TResource::operator=(Move(resource));
		return *this;
	}

	Ava_FORCEINLINE PolymorphicResource& operator=(const TResource& resource)
	{
		TResource::operator=(resource);
		return *this;
	}

	Ava_FORCEINLINE void* Allocate(uword size)
	{
		return TResource::Allocate(size);
	}

	Ava_FORCEINLINE void Deallocate(void* block, uword size)
	{
		TResource::Deallocate(block, size);
	}
};

template<typename T>
Ava_FORCEINLINE bool operator==(const PolymorphicResource<T>& lhs, const PolymorphicResource<T>& rhs)
{
	return static_cast<const T&>(lhs) == static_cast<const T&>(rhs);
}

template<typename T>
Ava_FORCEINLINE bool operator!=(const PolymorphicResource<T>& lhs, const PolymorphicResource<T>& rhs)
{
	return static_cast<const T&>(lhs) != static_cast<const T&>(rhs);
}

} // namespace Ava
