#pragma once

#if defined(_WIN32) || defined(_WIN64)
#	ifdef Ava_OS
#		error Ambiguous operating system
#	endif
#	define Ava_WINDOWS 1
#	define Ava_OS Windows
#endif

#if defined(__linux__)
#	ifdef Ava_OS
#		error Ambiguous operating system
#	endif
#	define Ava_LINUX 1
#	define Ava_OS Linux
#endif

#ifndef Ava_OS
#	error Unsupported operating system
#endif

#ifndef Ava_WINDOWS
#	define Ava_WINDOWS 0
#endif

#ifndef Ava_LINUX
#	define Ava_LINUX 0
#endif
