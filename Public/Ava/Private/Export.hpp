#pragma once

#include "Ava/Private/Platform/Export.hpp"

#ifdef Ava_CONFIG_DLL
#	ifdef Ava_CONFIG_EXPORT
#		define Ava_API Ava_DLL_EXPORT
#	else
#		define Ava_API Ava_DLL_IMPORT
#	endif
#else
#	define Ava_API
#endif
