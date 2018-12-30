#pragma once

#include "Ava/Misc.hpp"
#include "Ava/Private/Export.hpp"

namespace Ava::Debug {

Ava_API bool IsDebuggerPresent();

} // namespace Ava::Debugger

#ifdef Ava_CONFIG_DISABLE_DEBUGGING
#	define Ava_DEBUGBREAK() ((void)0)
#else
#	define Ava_DEBUGBREAK() ( ::Ava::Debug::IsDebuggerPresent() ? Ava_PLATFORM_DEBUGBREAK() : (void)0 )
#endif
