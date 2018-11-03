#include "Ava/Memory/Ref.hpp"

#include "Ava/Memory/AllocatorDeleter.hpp"
#include "Ava/Memory/RefCount.hpp"
#include "Ava/Types.hpp"
#include "Ava/Utility/Move.hpp"

#include "UnitTest/Allocator.hpp"
#include "UnitTest/InstanceCounter.hpp"

#include "doctest.h"

using namespace Ava;

namespace {

struct TestValue : UnitTest::InstanceCounter, RefCount
{
	i32 m_value;

	TestValue(i32 value)
		: m_value(value)
	{
	}
};

typedef Ref<TestValue, AllocatorDeleter<UnitTest::Allocator>> TestRef;

TEST_SUITE_BEGIN("Memory/Ref");

TEST_CASE("Construct")
{
	UnitTest::Allocator alloc;

	SUBCASE("Default")
	{
		TestRef ref;
		REQUIRE(ref == nullptr);
	}

	SUBCASE("Pointer")
	{
		TestRef ref(new(alloc) TestValue(1));

		REQUIRE(ref != nullptr);
		CHECK(ref->m_value == 1);
	}

	SUBCASE("Move")
	{
		TestRef source(new(alloc) TestValue(1));

		TestRef ref(Move(source));
		REQUIRE(ref != nullptr);
		CHECK(ref->m_value == 1);
	}

	SUBCASE("Copy")
	{
		TestRef source(new(alloc) TestValue(1));

		TestRef ref(Move(source));
		REQUIRE(ref != nullptr);
		CHECK(ref->m_value == 1);
	}
}

TEST_CASE("Assign")
{
	UnitTest::Allocator alloc;

	SUBCASE("To empty")
	{
		TestRef ref;

		SUBCASE("Nullptr")
		{
			ref = nullptr;
			REQUIRE(ref == nullptr);
		}

		SUBCASE("Pointer")
		{
			ref = new(alloc) TestValue(1);
			REQUIRE(ref != nullptr);
			CHECK(ref->m_value == 1);
		}

		SUBCASE("Move")
		{
			TestRef source(new(alloc) TestValue(1));

			ref = Move(source);
			REQUIRE(ref != nullptr);
			CHECK(ref->m_value == 1);
		}

		SUBCASE("Copy")
		{
			TestRef source(new(alloc) TestValue(1));
			
			ref = source;
			REQUIRE(ref != nullptr);
			CHECK(ref->m_value == 1);
		}
	}

	SUBCASE("To non-empty")
	{
		TestRef ref(new(alloc) TestValue(0));

		SUBCASE("Nullptr")
		{
			ref = nullptr;
			REQUIRE(ref == nullptr);
		}

		SUBCASE("Pointer")
		{
			ref = new(alloc) TestValue(1);
			REQUIRE(ref != nullptr);
			CHECK(ref->m_value == 1);
		}

		SUBCASE("Move")
		{
			TestRef source(new(alloc) TestValue(1));

			ref = source;
			REQUIRE(ref != nullptr);
			CHECK(ref->m_value == 1);
		}
	}
}

TEST_SUITE_END();

} // namespace
