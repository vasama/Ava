#pragma once

#include "Ava/Memory/Allocator.hpp"

namespace Ava::UnitTest {

struct Allocator : Ava::Allocator<Allocator>
{
	void* Allocate(uword size) const;
	void Deallocate(void* block, uword size) const;
};

constexpr bool operator==(const Allocator&, const Allocator&)
{
	return true;
}

constexpr bool operator!=(const Allocator&, const Allocator&)
{
	return false;
}

} // namespace Ava::UnitTest
