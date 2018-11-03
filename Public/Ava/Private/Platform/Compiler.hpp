#pragma once

#if defined(__clang__)
#	define Ava_CLANG 1
#	define Ava_COMPILER Clang
#elif defined(_MSC_VER)
#	define Ava_MSVC 1
#	define Ava_COMPILER Msvc
#	ifdef __INTELLISENSE__
#		define Ava_IDE 1
#	endif
#else
#	error Unsupported compiler
#endif

#ifndef Ava_CLANG
#	define Ava_CLANG 0
#endif

#ifndef Ava_MSVC
#	define Ava_MSVC 0
#endif

#ifndef Ava_IDE
#	define Ava_IDE 0
#endif
