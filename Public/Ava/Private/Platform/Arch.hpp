#pragma once

#if defined(Ava_X86) \
||  defined(__i386__) \
||  defined(__i486__) \
||  defined(__i586__) \
||  defined(__i686__) \
||  defined(_M_IX86)
#	ifndef Ava_X86
#		define Ava_X86 1
#	endif
#	define Ava_32 1
#	define Ava_ARCH x86
#elif defined(Ava_X64) \
||    defined(__x86_64) \
||    defined(__x86_64__) \
||    defined(__amd64) \
||    defined(__amd64__) \
||    defined(_M_X64)
#	ifndef Ava_X64
#		define Ava_X64 1
#	endif
#	define Ava_64 1
#	define Ava_ARCH x64
#else
#	error Unsupported architecture
#endif

#ifndef Ava_32
#	define Ava_32 0
#endif

#ifndef Ava_64
#	define Ava_64 0
#endif

#ifndef Ava_X86
#	define Ava_X86 0
#endif

#ifndef Ava_X64
#	define Ava_X64 0
#endif
