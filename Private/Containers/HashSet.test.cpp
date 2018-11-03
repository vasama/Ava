#include "Ava/Containers/HashSet.hpp"

#include "UnitTest/Allocator.hpp"
#include "UnitTest/InstanceCounter.hpp"

#include "doctest.h"

using namespace Ava;

namespace {

struct Val : UnitTest::InstanceCounter
{
	i32 m_value;
	i32 m_hash;

	Val(i32 value)
		: m_value(value), m_hash(value)
	{
	}

	Val(i32 value, i32 hash)
		: m_value(value), m_hash(hash)
	{
	}
};

bool operator==(const Val& lhs, const Val& rhs)
{
	return lhs.m_value == rhs.m_value;
}

struct Hasher
{
	uword Hash(const Val& val) const
	{
		return val.m_hash;
	}
};

/*
#define TEST_PREFIX "Local"
#define TEST_POLICY StoragePolicy::Local<16>
#include "HashSet.test.ipp"
*/

#define TEST_PREFIX "Remote"
#define TEST_POLICY StoragePolicy::Remote<UnitTest::Allocator>
#define TEST_REHASH
#include "HashSet.test.ipp"

} // namespace
