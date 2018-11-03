#pragma once

#include "Ava/Misc.hpp"

namespace Ava::Private::Memory_SmartPtr {

template<typename T>
struct Base
{
	T* m_ptr;
};

} // namespace Ava::Private::Memory_SmartPtr
