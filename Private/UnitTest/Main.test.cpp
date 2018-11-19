#include "UnitTest/Allocator.hpp"
#include "UnitTest/InstanceCounter.hpp"

#include "Ava/Memory/BlockListResource.hpp"
#include "Ava/Memory/CAllocator.hpp"
#include "Ava/Debug/AssertCallback.hpp"

#define DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

//HACK: Disable break, return
#undef DOCTEST_ASSERT_LOG_AND_REACT
#define DOCTEST_ASSERT_LOG_AND_REACT(b) b.log()

using namespace Ava;

namespace {

iword GMemoryUsage;
BlockListResource<CAllocator> GResource(32 * 1024 - 64);

iword GInstanceCount;

struct : doctest::IValidator
{
	virtual void test_case_start(const doctest::TestCaseData&) override
	{
	}

	virtual void test_case_end(const doctest::TestCaseValidation& validation) override
	{
		if (iword memoryUsage = GMemoryUsage)
			validation.addFailure() << "Leaked " << memoryUsage << " bytes.";

		if (iword instanceCount = GInstanceCount)
			validation.addFailure() << "Leaked " << instanceCount << " objects.";

		GMemoryUsage = 0;
		GInstanceCount = 0;

		GResource.Reset();
	}
} GValidator;

} // namespace

void* Ava::UnitTest::Allocator::Allocate(uword size) const
{
	GMemoryUsage += (iword)size;
	return GResource.Allocate(size);
}

void Ava::UnitTest::Allocator::Deallocate(void* block, uword size) const
{
	GMemoryUsage -= (iword)size;
	return GResource.Deallocate(block, size);
}

void Ava::UnitTest::InstanceCounter::Increment()
{
	++GInstanceCount;
}

void Ava::UnitTest::InstanceCounter::Decrement()
{
	--GInstanceCount;
}

Debug::AssertAction Ava::Debug::AssertCallback(Debug::AssertInfo* info)
{
	DOCTEST_ADD_FAIL_AT(info->File.Data(), info->Line, info->Expr.Data());
	return Debug::AssertAction::Break;
}

int main(int argc, const char* const* argv)
{
	doctest::Context context(argc, argv);
	context.addValidator(GValidator);
	context.run();
}
