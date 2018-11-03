#include "Ava/Containers/HashMap.hpp"

#include "Ava/Utility/MurmurHash.hpp"

#include "UnitTest/Allocator.hpp"
#include "UnitTest/InstanceCounter.hpp"

#include "doctest.h"

using namespace Ava;

namespace {

struct Key : UnitTest::InstanceCounter
{
	i32 m_value;
	i32 m_hash;

	Key(i32 value)
		: m_value(value), m_hash(value)
	{
	}

	Key(i32 value, i32 hash)
		: m_value(value), m_hash(hash)
	{
	}
};

struct Val : UnitTest::InstanceCounter
{
	i32 m_value;

	Val(i32 value)
		: m_value(value)
	{
	}
};

bool operator==(const Key& lhs, const Key& rhs)
{
	return lhs.m_value == rhs.m_value;
}

struct Hasher
{
	uword Hash(const Key& key) const
	{
		return key.m_hash;
	}
};

TEST_SUITE_BEGIN("Containers/HashMap");

TEST_CASE("CustomHasher")
{
	typedef BasicHasher<MurmurHash<>> Hasher;
	HashMap<i32, i32, Default, Hasher> map(Hasher(42));

	CHECK(map.Insert(7).Inserted);
	CHECK_NE(map.Find(7), nullptr);
	CHECK_UNARY(map.Remove(7));
}

/*
#define TEST_PREFIX "Local"
#define TEST_POLICY StoragePolicy::Local<16>
#include "HashMap.test.ipp"
*/

#define TEST_PREFIX "Remote"
#define TEST_POLICY StoragePolicy::Remote<UnitTest::Allocator>
#define TEST_REHASH
#include "HashMap.test.ipp"

TEST_SUITE_END();

} // namespace
