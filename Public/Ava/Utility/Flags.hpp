#pragma once

#include "Ava/Meta/Identity.hpp"
#include "Ava/Misc.hpp"

#include <type_traits>

namespace Ava {

template<typename T>
bool HasFlag(T flags, Identity<T> flag)
{
	return (flags & flag) != (T)0;
}

} // namespace Ava

#define Ava_DEFINE_FLAGS(flags) \
	constexpr Ava_FORCEINLINE flags operator|(flags a, flags b) \
	{ \
		typedef __underlying_type(flags) T; \
		return (flags)((T)a | (T)b); \
	} \
	constexpr Ava_FORCEINLINE flags operator&(flags a, flags b) \
	{ \
		typedef __underlying_type(flags) T; \
		return (flags)((T)a & (T)b); \
	} \
	constexpr Ava_FORCEINLINE flags operator~(flags a) \
	{ \
		typedef __underlying_type(flags) T; \
		return (flags)~(T)a; \
	} \
