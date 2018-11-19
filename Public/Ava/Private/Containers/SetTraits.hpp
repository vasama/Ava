#pragma once

#include "Ava/Misc.hpp"

namespace Ava::Private::Containers_ {

struct SetTraits
{
	template<typename T>
	using SelectKeyType = T;

	template<typename T>
	static Ava_FORCEINLINE T& SelectKey(T& value)
	{
		return value;
	}
};

} // namespace Ava::Private::Containers_
