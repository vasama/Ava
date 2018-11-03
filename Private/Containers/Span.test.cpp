#include "Ava/Containers/Span.hpp"

#include "Ava/Types.hpp"

#include "doctest.h"

using namespace Ava;

namespace {

TEST_SUITE_BEGIN("Containers/Span");

TEST_CASE("Construct")
{
	i32 array[4] = { 1, 2, 3, 4 };

	Span<i32> span = array;
	CHECK(span.Size() == 4);
}

TEST_CASE("Subscript")
{
	i32 array[4] = { 1, 2, 3, 4 };

	Span<i32> span = array;
	CHECK(span[0] == 1);
	CHECK(span[2] == 3);
}

TEST_CASE("Slice")
{
	i32 array[4] = { 1, 2, 3, 4 };

	Span<i32> span = array;

	SUBCASE("Implicit length")
	{
		span = span.Slice(1);
		REQUIRE(span.Size() == 3);

		CHECK(span[0] == 2);
		CHECK(span[2] == 4);
	}

	SUBCASE("Explicit length")
	{
		span = span.Slice(1, 2);
		REQUIRE(span.Size() == 2);

		CHECK(span[0] == 2);
		CHECK(span[1] == 3);
	}
}

TEST_CASE("Equality")
{
	i32 array1[4] = { 1, 2, 3, 4 };
	i32 array2[4] = { 1, 2, 3, 4 };

	Span<i32> span = array1;
	CHECK(span == Span<i32>(array1));
	CHECK(span != Span<i32>(array2));

	span = span.Slice(1);
	CHECK(span != Span<i32>(array1));
}

TEST_SUITE_END();

} // namespace
