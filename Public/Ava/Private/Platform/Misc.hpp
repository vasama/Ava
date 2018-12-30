#pragma once

#include "Ava/Preprocessor/Str.hpp"
#include "Ava/Private/Platform/Arch.hpp"
#include "Ava/Private/Platform/Compiler.hpp"
#include "Ava/Private/Platform/Os.hpp"

#include Ava_PP_STR(Ava/Private/Platform/Ava_COMPILER/Misc.ipp)

#define Ava_PRAGMA(...) Ava_PLATFORM_PRAGMA(__VA_ARGS__)

// Push macro definition on the macro stack.
#define Ava_PUSH_MACRO(x) Ava_PLATFORM_PRAGMA(push_macro(x))

// Pop macro definition from the macro stack.
#define Ava_POP_MACRO(x) Ava_PLATFORM_PRAGMA(pop_macro(x))

#ifndef Ava_PLATFORM_MSVC_WARNING
#	define Ava_PLATFORM_MSVC_WARNING(...)
#endif

#ifndef Ava_PLATFORM_CLANG_WARNING
#	define Ava_PLATFORM_CLANG_WARNING(...)
#endif

// Manipulate MSVC warnings.
#define Ava_MSVC_WARNING(...) Ava_PLATFORM_MSVC_WARNING(__VA_ARGS__)

// Manipulate Clang warnings.
#define Ava_CLANG_WARNING(...) Ava_PLATFORM_CLANG_WARNING(__VA_ARGS__)

#ifdef Ava_CONFIG_DISABLE_FORCEINLINE
	// Force function inlining.
#	define Ava_FORCEINLINE inline
#else
	// Force function inlining.
#	define Ava_FORCEINLINE Ava_PLATFORM_FORCEINLINE
#endif

// Prevent function inlining.
#define Ava_FORCENOINLINE Ava_PLATFORM_FORCENOINLINE

// Hint the compiler to assume the given expression.
#define Ava_ASSUME(...) Ava_PLATFORM_ASSUME(__VA_ARGS__)

// Declare code path as unreachable.
#define Ava_UNREACHABLE() Ava_PLATFORM_UNREACHABLE()

// Hint the compiler to expect the given expression.
#define Ava_LIKELY(...) Ava_PLATFORM_LIKELY(__VA_ARGS__)

// Hint the compiler not to expect the given expression.
#define Ava_UNLIKELY(...) Ava_PLATFORM_UNLIKELY(__VA_ARGS__)

// C99 restrict. Pointer is not aliased.
#define Ava_RESTRICT Ava_PLATFORM_RESTRICT

// Break into debugger.
#define Ava_DEBUGBREAK() Ava_PLATFORM_DEBUGBREAK()

// Mark local variables as unused.
#define Ava_UNUSED(...) ((void)sizeof(__VA_ARGS__))
