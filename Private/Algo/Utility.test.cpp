#include "Ava/Algo/Utility.hpp"

#include "doctest.h"

using namespace Ava;

#define APPLY(algo, first, count, ...) \
	SUBCASE("Count") { Algo::algo(first, count, ##__VA_ARGS__); } \
	SUBCASE("Iterator") { Algo::algo(first, first + count, ##__VA_ARGS__); }

#define APPLYX(algo, pre, first, count, ...) \
	SUBCASE("Count") { Algo::algo(pre, first, count, ##__VA_ARGS__); } \
	SUBCASE("Iterator") { Algo::algo(pre, first, first + count, ##__VA_ARGS__); }

namespace {

struct Wrap
{
	i32 Value;

	Wrap()
		: Value(0)
	{
	}

	Wrap(i32 value)
		: Value(value)
	{
	}

	Wrap(Wrap&& w)
		: Value(w.Value)
	{
		w.Value = 0;
	}

	Wrap(const Wrap& w)
		: Value(w.Value)
	{
	}
};

TEST_SUITE_BEGIN("Algo/Utility");

TEST_CASE("DefaultConstruct")
{
	SUBCASE("Trivial")
	{
		i32 array[1];
		array[0] = 42;

		APPLY(DefaultConstruct, array, 1);
		CHECK(array[0] == 42);
	}

	SUBCASE("Non-trivial")
	{
		Wrap array[1];
		array[0].Value = 42;

		APPLY(DefaultConstruct, array, 1);
		CHECK(array[0].Value == 0);
	}
}

TEST_CASE("ValueConstruct")
{
	i32 array[1];
	array[0] = 42;

	APPLY(ValueConstruct, array, 1);
	CHECK(array[0] == 0);
}

TEST_CASE("Construct")
{
	i32 array[1];
	array[0] = 0;

	APPLY(Construct, array, 1, 42);
	CHECK(array[0] == 42);
}

TEST_CASE("MoveConstruct")
{
	Wrap src[1];
	src[0].Value = 42;

	Wrap array[1];

	APPLYX(MoveConstruct, array, src, 1);
	CHECK(src[0].Value == 0);
	CHECK(array[0].Value == 42);
}

TEST_CASE("CopyConstruct")
{
	Wrap src[1];
	src[0].Value = 42;

	Wrap array[1];

	APPLYX(CopyConstruct, array, src, 1);
	CHECK(src[0].Value == 42);
	CHECK(array[0].Value == 42);
}

TEST_SUITE_END();

} // namespace
