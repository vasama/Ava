#include "Ava/Algo/LinearSearch.hpp"

#include "Ava/Types.hpp"

#include "doctest.h"

using namespace Ava;

static const i32 Ints[] = {
	2, 3, 5, 7, 11, 13, 17, 19,
	23, 29, 31, 37, 41, 43, 47, 53,
	59, 61, 67, 71, 73, 79, 83, 89,
	97, 101, 103, 107, 109, 113, 127, 131,
};

TEST_SUITE_BEGIN("Algo/LinearSearch");

TEST_CASE("LinearSearch")
{
	SUBCASE("Predicate")
	{
		SUBCASE("Empty")
		{
			auto* iter = Algo::LinearSearch(Begin(Ints),
				Begin(Ints), [](auto x) { return x > 10; });
			CHECK_EQ(iter, Begin(Ints));
		}

		SUBCASE("Extant")
		{
			auto* iter = Algo::LinearSearch(Begin(Ints),
				End(Ints), [](auto x) { return x > 10; });
			REQUIRE_NE(iter, End(Ints));
			CHECK_EQ(*iter, 11);
		}

		SUBCASE("Inextant")
		{
			auto* iter = Algo::LinearSearch(Begin(Ints),
				End(Ints), [](auto x) { return x < 0; });
			CHECK_EQ(iter, End(Ints));
		}
	}

	SUBCASE("Value")
	{
		SUBCASE("Empty")
		{
			auto* iter = Algo::LinearSearch(
				Begin(Ints), Begin(Ints), 41);
			CHECK_EQ(iter, Begin(Ints));
		}

		SUBCASE("Extant")
		{
			auto* iter = Algo::LinearSearch(
				Begin(Ints), End(Ints), 41);
			REQUIRE_NE(iter, End(Ints));
			CHECK_EQ(iter, &Ints[12]);
		}

		SUBCASE("Inextant")
		{
			auto* iter = Algo::LinearSearch(
				Begin(Ints), End(Ints), 42);
			CHECK_EQ(iter, End(Ints));
		}
	}
}

TEST_CASE("Any")
{
	SUBCASE("Empty")
	{
		CHECK_UNARY_FALSE(Algo::Any(Begin(Ints),
			Begin(Ints), [](auto x) { return x > 10; }));
	}

	SUBCASE("True")
	{
		CHECK_UNARY(Algo::Any(Begin(Ints),
			End(Ints), [](auto x) { return x > 10; }));
	}

	SUBCASE("False")
	{
		CHECK_UNARY_FALSE(Algo::Any(Begin(Ints),
			End(Ints), [](auto x) { return x < 0; }));
	}
}

TEST_CASE("All")
{
	SUBCASE("Empty")
	{
		CHECK_UNARY(Algo::All(Begin(Ints),
			Begin(Ints), [](auto x) { return x < 0; }));
	}

	SUBCASE("True")
	{
		CHECK_UNARY(Algo::All(Begin(Ints),
			End(Ints), [](auto x) { return x > 0; }));
	}

	SUBCASE("False")
	{
		CHECK_UNARY_FALSE(Algo::All(Begin(Ints),
			End(Ints), [](auto x) { return x < 128; }));
	}
}

TEST_CASE("None")
{
	SUBCASE("Empty")
	{
		CHECK_UNARY(Algo::None(Begin(Ints),
			Begin(Ints), [](auto x) { return x > 0; }));
	}

	SUBCASE("True")
	{
		CHECK_UNARY(Algo::None(Begin(Ints),
			End(Ints), [](auto x) { return x < 0; }));
	}

	SUBCASE("False")
	{
		CHECK_UNARY_FALSE(Algo::None(Begin(Ints),
			End(Ints), [](auto x) { return x > 128; }));
	}
}

TEST_SUITE_END();
