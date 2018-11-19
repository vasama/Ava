#include "Ava/Debug/Assert.hpp"
#include "Ava/Debug/AssertCallback.hpp"

#include "Ava/Containers/StringSpan.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"

#include <stdlib.h>

using namespace Ava;

#if Ava_CONFIG_ASSERT > 0
static bool GHandling = false;

bool Ava::Private::Debug_Assert::Fail(const char* expr, const char* file, i32 line, const char* fmt, ...)
{
	if (GHandling)
		::abort();

	GHandling = true;

	Debug::AssertInfo info;

	info.Expr = StringSpan::FromCString(expr);
	info.File = StringSpan::FromCString(file);
	info.Line = line;

	if (fmt)
	{
		//TODO: Assert message formatting
		Ava_UNUSED(fmt);
	}

	switch (Debug::AssertCallback(&info))
	{
	case Debug::AssertAction::Abort:
		::abort();

	case Debug::AssertAction::Break:
		GHandling = false;
		return true;

	case Debug::AssertAction::Continue:
		GHandling = false;
		return false;
	}

	Ava_UNREACHABLE();
}
#endif
