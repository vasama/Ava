#pragma once

#include "Ava/Memory/AllocatorResource.hpp"
#include "Ava/Memory/DefaultAllocator.hpp"
#include "Ava/Memory/PolymorphicAllocator.hpp"
#include "Ava/Private/Export.hpp"

namespace Ava {

typedef AllocatorResource<DefaultAllocator> DefaultMemoryResource;
extern Ava_API PolymorphicResource<DefaultMemoryResource> GDefaultMemoryResource;

} // namespace Ava
