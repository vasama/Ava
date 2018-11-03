#pragma once

#include "Ava/Misc.hpp"
#include "Ava/Meta/EnableIf.hpp"
#include "Ava/Meta/Traits.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Types.hpp"

template<typename THash, typename T>
Ava_FORCEINLINE auto Ava_Ext_Hash(THash& hash, const T& value)
	-> Ava::EnableIf<Ava::IsTriviallyHashable<Ava::Decay<T>>>
{
#if !Ava_X86 && !Ava_X64
#	error Pointers are assumed to be integers
#endif

	if constexpr (Ava::IsInteger<Ava::Decay<T>> || Ava::IsPointer<Ava::Decay<T>>)
	{
		if constexpr (sizeof(T) > sizeof(Ava::uword))
		{
			static_assert(sizeof(T) == sizeof(Ava::u64) &&
				sizeof(Ava::uword) == sizeof(Ava::u32));

			hash.MixWord((Ava::uword)((Ava::u64)value >> 32));
		}

		hash.MixWord((Ava::uword)value);
	}
	else
	{
		hash.Mix((void*)&value, sizeof(value));
	}
}

template<typename THash, typename T, Ava::uword TSize>
Ava_FORCEINLINE void Ava_Ext_Hash(THash& hash, const T(&array)[TSize])
{
	if constexpr (Ava::IsTriviallyHashable<Ava::Decay<T>>)
	{
		hash.Mix((void*)array, sizeof(array));
	}
	else
	{
		for (const T& x : array)
			Ava_Ext_Hash(hash, x);
	}
}
