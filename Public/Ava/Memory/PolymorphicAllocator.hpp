#pragma once

#include "Ava/Memory/ResourceAllocator.hpp"
#include "Ava/Memory/PolymorphicResource.hpp"

namespace Ava {

typedef ResourceAllocator<VirtualResource> PolymorphicAllocator;

} // namespace Ava
