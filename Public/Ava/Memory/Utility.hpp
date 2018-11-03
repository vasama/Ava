#pragma once

#include "Ava/Memory/Memory.hpp"
#include "Ava/Meta/Identity.hpp"
#include "Ava/Meta/Traits.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Utility/Forward.hpp"
#include "Ava/Utility/Move.hpp"

#include <new>

namespace Ava {

template<typename T>
Ava_FORCEINLINE void ValueConstruct(RemoveConst<T>* ptr)
{
	if constexpr (IsZeroConstructible<T>)
	{
		Memory::Zero(ptr, sizeof(T));
	}
	else
	{
		::new(ptr) T();
	}
}

template<typename T>
Ava_FORCEINLINE void DefaultConstruct(RemoveConst<T>* ptr)
{
	if constexpr (IsTriviallyConstructible<T>)
	{
		Ava_UNUSED(ptr);
	}
	else
	{
		ValueConstruct<T>(ptr);
	}
}

template<typename T>
Ava_FORCEINLINE void MoveConstruct(RemoveConst<T>* ptr, RemoveConst<T>&& src)
{
	if constexpr (IsTriviallyCopyable<T>)
	{
		Memory::Copy(ptr, &src, sizeof(T));
	}
	else
	{
		::new(ptr) T(Move(src));
	}
}

template<typename T>
Ava_FORCEINLINE void CopyConstruct(RemoveConst<T>* ptr, const Identity<T>& src)
{
	if constexpr (IsTriviallyCopyable<T>)
	{
		Memory::Copy(ptr, &src, sizeof(T));
	}
	else
	{
		::new(ptr) T(src);
	}
}

namespace Private::Memory_Utility {

template<typename T>
Ava_FORCEINLINE void Construct(T* ptr, T&& src)
{
	MoveConstruct<T>(ptr, Move(src));
}

template<typename T>
Ava_FORCEINLINE void Construct(T* ptr, T& src)
{
	CopyConstruct<T>(ptr, src);
}

template<typename T>
Ava_FORCEINLINE void Construct(T* ptr, const T& src)
{
	CopyConstruct<T>(ptr, src);
}

template<typename T, typename... TArgs>
Ava_FORCEINLINE void Construct(T* ptr, TArgs&&... args)
{
	::new(ptr) T(Forward<TArgs>(args)...);
}

} // namespace Private::Memory_Utility

template<typename T, typename... TArgs>
Ava_FORCEINLINE void Construct(RemoveConst<T>* ptr, TArgs&&... args)
{
	Private::Memory_Utility::Construct<T>(ptr, Forward<TArgs>(args)...);
}

template<typename T>
Ava_FORCEINLINE void Relocate(RemoveConst<T>* ptr, RemoveConst<T>* src)
{
	if constexpr (IsTriviallyRelocatable<T>)
	{
		Memory::Copy(ptr, src, sizeof(T));
	}
	else
	{
		new(ptr) T(Move(*src));
		src->~T();
	}
}

template<typename T>
Ava_FORCEINLINE void Destroy(T* ptr)
{
	if constexpr (IsTriviallyDestructible<T>)
	{
		Ava_UNUSED(ptr);
	}
	else
	{
		ptr->~T();
	}
}

template<typename T>
void Destroy(const T*) = delete;

} // namespace Ava
