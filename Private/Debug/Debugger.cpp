#include "Ava/Debug/Debugger.hpp"

#include "Ava/Misc.hpp"

#if Ava_WINDOWS
#	include "../Windows.hpp"
#endif

bool Ava::Debug::IsDebuggerPresent()
{
#if Ava_WINDOWS
	return ::IsDebuggerPresent();
#else
	return false;
#endif
}
