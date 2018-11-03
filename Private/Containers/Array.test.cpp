#include "Ava/Containers/Array.hpp"

#include "UnitTest/Allocator.hpp"
#include "UnitTest/InstanceCounter.hpp"

#include "doctest.h"

using namespace Ava;

namespace {

struct Val : UnitTest::InstanceCounter
{
	i32 m_value;

	Val()
		: m_value(0)
	{
	}

	Val(i32 value)
		: m_value(value)
	{
	}
};

TEST_SUITE_BEGIN("Containers/Array");

#define TEST_PROLOGUE \
	typedef InplaceArray<Val, 10> Array;
#define TEST_PREFIX "Local"
#include "Array.test.ipp"

#define TEST_PROLOGUE \
	typedef Array<Val, UnitTest::Allocator> Array;
#define TEST_PREFIX "Remote"
#include "Array.test.ipp"

TEST_SUITE_END();

} // namespace
