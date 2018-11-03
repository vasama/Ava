#pragma once

#include "Ava/Misc.hpp"

namespace Ava {

struct DefaultDeleter
{
	template<typename T>
	Ava_FORCEINLINE void operator()(T* ptr)
	{
		delete ptr;
	}
};

} // namespace Ava
