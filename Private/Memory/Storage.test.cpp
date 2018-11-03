#include "Ava/Memory/Storage.hpp"

#include "Ava/Types.hpp"

#include "UnitTest/Allocator.hpp"
#include "UnitTest/InstanceCounter.hpp"

#include "doctest.h"

using namespace Ava;

namespace {

struct TestValue : UnitTest::InstanceCounter
{
	i32 m_value;

	TestValue()
		: m_value(0)
	{
	}

	TestValue(i32 value)
		: m_value(value)
	{
	}
};

TEST_SUITE_BEGIN("Memory/Storage");

TEST_CASE("Manual")
{
	Storage<TestValue> storage;
	storage.Construct(1);
	storage.Destroy();
}

TEST_CASE("Automatic")
{
	SUBCASE("Construct")
	{
		Storage<TestValue, StorageOptions::Construct> storage;
		storage.Destroy();
	}

	SUBCASE("Destroy")
	{
		Storage<TestValue, StorageOptions::Destroy> storage;
		storage.Construct();
	}

	SUBCASE("Construct | Destroy")
	{
		Storage<TestValue, StorageOptions::Construct | StorageOptions::Destroy> storage;
	}
}

TEST_SUITE_END();

} // namespace
