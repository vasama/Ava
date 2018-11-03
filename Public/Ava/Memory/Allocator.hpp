#pragma once

#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"

namespace Ava {

template<typename TImplementation>
class Allocator
{
public:
	Ava_FORCEINLINE void* Allocate(uword size) const
	{
		return static_cast<const TImplementation*>(this)->Allocate(size);
	}

	Ava_FORCEINLINE void Deallocate(void* block, uword size) const
	{
		static_cast<const TImplementation*>(this)->Deallocate(block, size);
	}

protected:
	Allocator() = default;
};

template<typename TImplementation, typename T>
Ava_FORCEINLINE void Delete(const Allocator<TImplementation>& allocator, T* object)
{
	object->~T(); allocator.Deallocate(object, sizeof(T));
}

} // namespace Ava

template<typename T>
Ava_FORCEINLINE void* operator new(Ava::uword size, const Ava::Allocator<T>& allocator)
{
	return allocator.Allocate(size);
}

template<typename T>
void* operator new(Ava::uword size, const Ava::Allocator<T>* allocator) = delete;
