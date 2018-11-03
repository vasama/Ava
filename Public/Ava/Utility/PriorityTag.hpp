#pragma once

#include "Ava/Types.hpp"

namespace Ava {

template<u32 TPriority>
struct PriorityTag : PriorityTag<TPriority - 1>
{
};

template<>
struct PriorityTag<0>
{
};

} // namespace Ava
