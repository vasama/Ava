#pragma once

#include "Ava/Meta/EnableIf.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"

#include "Ava/Private/Platform/Math.hpp"

namespace Ava {

namespace Math {

#include "Ava/Private/Platform/Math_Bits.ipp"

template<typename T>
constexpr EnableIf<IsUnsigned<T>, T> HighBit = ~(~(T)0 >> 1);

template<typename T>
Ava_FORCEINLINE EnableIf<IsUnsigned<T>, T> IsPowerOfTwoOrZero(T x)
{
	return (x & (x - 1)) == 0;
}

template<typename T>
Ava_FORCEINLINE EnableIf<IsUnsigned<T>, T> IsPowerOfTwo(T x)
{
	return Popcnt(x) == 1;
}

template<typename T>
Ava_FORCEINLINE EnableIf<IsUnsigned<T>, T> NextPowerOfTwo(T x)
{
	if (IsPowerOfTwoOrZero(x))
		return x;

	u32 lz = (u32)Lzcnt(x);
	return HighBit<T> >> (lz - 1);
}

} // namespace Math

namespace Private::Math_Constexpr {

template<typename T>
constexpr T NextPowerOfTwo(T x, u32 i)
{
	return x & Math::HighBit<T>
		? Math::HighBit<T> >> (i - 1)
		: NextPowerOfTwo(x << 1, i + 1);
}

} // namespace Private::Math_Constexpr

namespace Math::Constexpr {

template<typename T>
constexpr EnableIf<IsUnsigned<T>, T> IsPowerOfTwo(T x)
{
	return x != 0 && (x & (x - 1)) == 0;
}

template<typename T>
constexpr EnableIf<IsUnsigned<T>, T> NextPowerOfTwo(T x)
{
	return (x & (x - 1)) == 0 ? x :
		Private::Math_Constexpr::NextPowerOfTwo(x, 0);
}

} // namespace Math::Constexpr

} // namespace Ava
