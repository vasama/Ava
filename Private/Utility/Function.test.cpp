#include "Ava/Utility/Function.hpp"

#include "Ava/Utility/Move.hpp"
#include "UnitTest/Allocator.hpp"
#include "UnitTest/InstanceCounter.hpp"

#include "doctest.h"

using namespace Ava;

namespace {

typedef i32 Signature(i32);

template<typename T>
struct Wrap : UnitTest::InstanceCounter
{
	T m_value;

	Wrap(T value)
		: m_value(value)
	{
	}
};

template<typename T>
Ava_FORCENOINLINE T Pessimize(T value)
{
	return value;
}

struct Callable0
{
	i32 operator()(i32 x) const
	{
		return x;
	}
};

struct Callable1
{
	i32 operator()(i32 x) const
	{
		return x + m_a;
	}

	i32 m_a;
};

struct Callable2
{
	i32 operator()(i32 x) const
	{
		return x + m_a + m_b;
	}

	i32 m_a;
	i32 m_b;
};

} // namespace

#define SUITE_NAME "Local"
#define FUNCTION_TYPE InplaceFunction<Signature, sizeof(void*) * 2>
#include "Function.test.ipp"

#define SUITE_NAME "Small"
#define FUNCTION_TYPE SmallFunction<Signature, sizeof(void*) * 1>
#include "Function.test.ipp"

#define SUITE_NAME "Remote"
#define FUNCTION_TYPE Function<Signature>
#include "Function.test.ipp"
