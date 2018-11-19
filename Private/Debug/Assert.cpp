#include "Ava/Debug/Assert.hpp"
#include "Ava/Debug/AssertCallback.hpp"

#include "Ava/Containers/StringSpan.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"

#include <stdlib.h>

using namespace Ava;

#if Ava_CONFIG_ASSERT > 0
bool Ava::Private::Debug_Assert::Fail(const char* expr, const char* file, i32 line, const char* fmt, ...)
{
	Debug::AssertInfo info;

	info.Expr = StringSpan<char>::FromCString(expr);
	info.File = StringSpan<char>::FromCString(file);
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
		return true;

	case Debug::AssertAction::Continue:
		return false;
	}

	Ava_UNREACHABLE();
}
#endif
