#pragma once

#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Private/Export.hpp"
#include "Ava/Types.hpp"

namespace Ava {

class VirtualResource
{
	struct VTable
	{
		void*(*m_allocate)(VirtualResource*, uword);
		void(*m_deallocate)(VirtualResource*, void*, uword);
		bool(*m_equals)(const VirtualResource*, const VirtualResource*);
	};

public:
	Ava_FORCEINLINE void* Allocate(uword size)
	{
		return m_vptr->m_allocate(this, size);
	}

	Ava_FORCEINLINE void Deallocate(void* block, uword size)
	{
		if (Ava_LIKELY(block))
			m_vptr->m_deallocate(this, block, size);
	}

	Ava_FORCEINLINE bool Equals(const VirtualResource& other)
	{
		const VTable* vptr = m_vptr;
		return other.m_vptr == vptr && vptr->m_equals(this, &other);
	}

	Ava_API VirtualResource* GetDefaultResource();

protected:
	template<typename TResource>
	Ava_FORCEINLINE VirtualResource(TResource*)
		Ava_MSVC_WARNING(suppress: 4268)
		: m_vptr(&s_vtable<TResource>)
	{
		static_assert(IsBaseOf<VirtualResource, TResource>);
	}

private:
	const VTable* m_vptr;

	template<typename T>
	static const VTable s_vtable;
};

} // namespace Ava

template<typename T>
inline const Ava::VirtualResource::VTable Ava::VirtualResource::s_vtable = {
	[](Ava::VirtualResource* self, uword size) -> void* {
		return static_cast<T*>(self)->Allocate(size); },

	[](Ava::VirtualResource* self, void* block, uword size) -> void {
		static_cast<T*>(self)->Deallocate(block, size); },

	[](const Ava::VirtualResource* lhs, const Ava::VirtualResource* rhs) -> bool {
		return *static_cast<const T*>(lhs) == *static_cast<const T*>(rhs); }
};
