#include "Ava/Memory/VirtualResource.hpp"

#include "Ava/Memory/DefaultResource.hpp"

using namespace Ava;

VirtualResource* Ava::VirtualResource::GetDefaultResource()
{
	return &GDefaultMemoryResource;
}
