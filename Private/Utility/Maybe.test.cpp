#include "Ava/Utility/Maybe.hpp"

#include "Ava/Types.hpp"
#include "Ava/Utility/Move.hpp"

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

TEST_SUITE_BEGIN("Utility/Maybe");

TEST_CASE("Construct")
{
	SUBCASE("Default")
	{
		Maybe<TestValue> maybe;
		REQUIRE(!maybe.HasValue());
	}

	SUBCASE("MaybeNot")
	{
		Maybe<TestValue> maybe = MaybeNot;
		REQUIRE(!maybe.HasValue());
	}

	SUBCASE("Explicit")
	{
		Maybe<TestValue> maybe(TestValue(1));
		REQUIRE(maybe.HasValue());
		CHECK(maybe->m_value == 1);
	}

	SUBCASE("Implicit")
	{
		Maybe<TestValue> maybe(1);
		REQUIRE(maybe.HasValue());
		CHECK(maybe->m_value == 1);
	}

	SUBCASE("Copy")
	{
		Maybe<TestValue> source(TestValue(1));

		Maybe<TestValue> maybe(source);
		REQUIRE(maybe.HasValue());
		CHECK(maybe->m_value == 1);
	}

	SUBCASE("Move")
	{
		Maybe<TestValue> source(TestValue(1));

		Maybe<TestValue> maybe(Move(source));
		CHECK(!source.HasValue());
		REQUIRE(maybe.HasValue());
		CHECK(maybe->m_value == 1);
	}
}

TEST_CASE("Assign")
{
	SUBCASE("To empty")
	{
		Maybe<TestValue> maybe;

		SUBCASE("MaybeNot")
		{
			maybe = MaybeNot;
			REQUIRE(!maybe.HasValue());
		}

		SUBCASE("Explicit")
		{
			maybe = TestValue(1);
			REQUIRE(maybe.HasValue());
			CHECK(maybe->m_value == 1);
		}

		SUBCASE("Implicit")
		{
			maybe = 1;
			REQUIRE(maybe.HasValue());
			CHECK(maybe->m_value == 1);
		}
	}

	SUBCASE("To non-empty")
	{
		Maybe<TestValue> maybe(0);

		SUBCASE("MaybeNot")
		{
			maybe = MaybeNot;
			REQUIRE(!maybe.HasValue());
		}

		SUBCASE("Explicit")
		{
			maybe = TestValue(1);
			REQUIRE(maybe.HasValue());
			CHECK(maybe->m_value == 1);
		}

		SUBCASE("Implicit")
		{
			maybe = 1;
			REQUIRE(maybe.HasValue());
			CHECK(maybe->m_value == 1);
		}
	}
}

TEST_CASE("Reset")
{
	SUBCASE("Empty")
	{
		Maybe<TestValue> maybe;

		maybe.Reset();
		REQUIRE(!maybe.HasValue());
	}

	SUBCASE("Non-empty")
	{
		Maybe<TestValue> maybe(0);

		maybe.Reset();
		REQUIRE(!maybe.HasValue());
	}
}

TEST_CASE("Nested")
{
	Maybe<Maybe<TestValue>> maybe;
	REQUIRE(!maybe.HasValue());

	maybe = Maybe<TestValue>(TestValue(1));
	REQUIRE(maybe.HasValue());
	REQUIRE(maybe->HasValue());
	CHECK((*maybe)->m_value == 1);

	maybe->Reset();
	REQUIRE(maybe.HasValue());
	REQUIRE(!maybe->HasValue());

	maybe.Reset();
	REQUIRE(!maybe.HasValue());
}

TEST_SUITE_END();

} // namespace
