#pragma once

#include "Ava/Debug/Assert.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Meta/EnableIf.hpp"
#include "Ava/Meta/Traits.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Utility/Move.hpp"
#include "Ava/Utility/PriorityTag.hpp"

namespace Ava {

namespace Private::Utility_Swap {

template<typename T>
Ava_FORCEINLINE auto Swap(T& a, T& b, PriorityTag<2>)
	-> decltype(Ava_Ext_Swap(a, b), void())
{
	Ava_Ext_Swap(a, b);
}

template<typename T>
Ava_FORCEINLINE auto Swap(T& a, T& b, PriorityTag<2>)
	-> decltype(a.Swap(b), void())
{
	a.Swap(b);
}

//TODO: Swap specialization using trivial relocate
/* template<typename T>
Ava_FORCEINLINE auto Swap(T& a, T& b, PriorityTag<1>)
	-> EnableIf<IsTriviallyRelocatable<Decay<T>>>
{
} */

template<typename T>
Ava_FORCEINLINE void Swap(T& a, T& b, PriorityTag<0>)
{
	T t = Move(b);
	b = Move(a);
	b = Move(t);
}

} // namespace Private::Utility_Swap

template<typename T>
Ava_FORCEINLINE void Swap(T& a, T& b)
{
	Private::Utility_Swap::Swap<T>(a, b, PriorityTag<1>());
}

} // namespace Ava
