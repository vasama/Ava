#pragma once

#include "Ava/Meta/Condition.hpp"
#include "Ava/Misc.hpp"

#include <type_traits>

namespace Ava {

// Use std::is_trivially_copyable to ensure the compiler
// agrees on the triviality of copying the parameter.

#define Ava_TEMP Condition<sizeof(T) <= sizeof(void*) \
	&& std::is_trivially_copyable_v<T>, const T, const T&>

#if Ava_MSVC
namespace Private::Utility_ {
template<typename T>
struct ConstParam
{
	typedef Ava_TEMP Type;
};
} // namespace Private::Utility_

template<typename T>
using ConstParam = typename Private::Utility_::ConstParam<T>::Type;
#else
template<typename T>
using ConstParam = Ava_TEMP;
#endif

#undef Ava_TEMP

template<typename T>
struct ConstParamWrapper
{
	explicit Ava_FORCEINLINE ConstParamWrapper(const T& value)
		: Value(value)
	{
	}

	ConstParam<T> Value;
};

} // namespace Ava
