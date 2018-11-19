#include "Ava/Containers/StringSpan.hpp"

#include "Ava/Types.hpp"

#include "doctest.h"

using namespace Ava;

namespace {

TEST_SUITE_BEGIN("Containers/StringSpan");

TEST_CASE("Construct")
{
	StringSpan span = "test";
	CHECK(span.Length() == 4);
}

TEST_CASE("Subscript")
{
	StringSpan span = "test";
	CHECK(span[0] == 't');
	CHECK(span[2] == 's');
}

TEST_CASE("Slice")
{
	StringSpan span = "test";

	SUBCASE("Implicit length")
	{
		span = span.Slice(1);
		REQUIRE(span.Length() == 3);

		CHECK(span[0] == 'e');
		CHECK(span[2] == 't');
	}

	SUBCASE("Explicit length")
	{
		span = span.Slice(1, 2);
		REQUIRE(span.Length() == 2);

		CHECK(span[0] == 'e');
		CHECK(span[1] == 's');
	}
}

TEST_CASE("Equality")
{
	StringSpan span = "testtest";
	CHECK(span.Slice(0, 4) == span.Slice(4, 4));
	CHECK(span.Slice(0, 4) != span.Slice(2, 4));
}

#if 0
TEST_CASE("Ordering")
{
	StringSpan span1, span2;

	SUBCASE("Equal")
	{
		span2 = span1 = "test";

		CHECK(span1 <= span2);
		CHECK(span1 >= span2);
		CHECK_FALSE(span1 < span2);
		CHECK_FALSE(span1 > span2);
	}

	SUBCASE("Not equal")
	{
		SUBCASE("Simple")
		{
			span1 = "cat";
			span2 = "dog";
		}

		SUBCASE("Common sequence")
		{
			span1 = "test";
			span2 = "testes";
		}

		CHECK(span1 < span2);
		CHECK_FALSE(span1 >= span2);
	}
}
#endif

TEST_SUITE_END();

} // namespace
