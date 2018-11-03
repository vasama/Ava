#include "Ava/Memory/Owner.hpp"

#include "Ava/Memory/AllocatorDeleter.hpp"
#include "Ava/Types.hpp"
#include "Ava/Utility/Move.hpp"

#include "UnitTest/Allocator.hpp"
#include "UnitTest/InstanceCounter.hpp"

#include "doctest.h"

using namespace Ava;

namespace {

struct TestValue : UnitTest::InstanceCounter
{
	i32 m_value;

	TestValue(i32 value)
		: m_value(value)
	{
	}
};

typedef Owner<TestValue, AllocatorDeleter<UnitTest::Allocator>> TestOwner;

TEST_SUITE_BEGIN("Memory/Owner");

TEST_CASE("Construct")
{
	UnitTest::Allocator alloc;

	SUBCASE("Default")
	{
		TestOwner owner;
		REQUIRE(owner == nullptr);
	}

	SUBCASE("Pointer")
	{
		TestOwner owner(new(alloc) TestValue(1));
		REQUIRE(owner != nullptr);
		CHECK(owner->m_value == 1);
	}

	SUBCASE("Move")
	{
		TestOwner source(new(alloc) TestValue(1));

		TestOwner owner(Move(source));
		REQUIRE(owner != nullptr);
		CHECK(owner->m_value == 1);
	}
}

TEST_CASE("Assign")
{
	UnitTest::Allocator alloc;

	SUBCASE("To empty")
	{
		TestOwner owner;

		SUBCASE("Nullptr")
		{
			owner = nullptr;
			REQUIRE(owner == nullptr);
		}

		SUBCASE("Pointer")
		{
			owner = new(alloc) TestValue(1);
			REQUIRE(owner != nullptr);
			CHECK(owner->m_value == 1);
		}

		SUBCASE("Move")
		{
			TestOwner source(new(alloc) TestValue(1));

			owner = Move(source);
			REQUIRE(owner != nullptr);
			CHECK(owner->m_value == 1);
		}
	}

	SUBCASE("To non-empty")
	{
		TestOwner owner(new(alloc) TestValue(0));

		SUBCASE("Nullptr")
		{
			owner = nullptr;
			REQUIRE(owner == nullptr);
		}

		SUBCASE("Pointer")
		{
			owner = new(alloc) TestValue(1);
			REQUIRE(owner != nullptr);
			CHECK(owner->m_value == 1);
		}

		SUBCASE("Move")
		{
			TestOwner source(new(alloc) TestValue(1));

			owner = Move(source);
			REQUIRE(owner != nullptr);
			CHECK(owner->m_value == 1);
		}
	}
}

TEST_SUITE_END();

} // namespace
