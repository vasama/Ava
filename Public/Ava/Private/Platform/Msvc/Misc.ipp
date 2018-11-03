#include <intrin.h>

#define Ava_PLATFORM_PRAGMA(...)          __pragma(__VA_ARGS__)
#define Ava_PLATFORM_MSVC_WARNING(...)    __pragma(warning(__VA_ARGS__))
#define Ava_PLATFORM_FORCEINLINE          __forceinline
#define Ava_PLATFORM_FORCENOINLINE        __declspec(noinline)
#define Ava_PLATFORM_ASSUME(...)          __assume(__VA_ARGS__)
#define Ava_PLATFORM_UNREACHABLE()        __assume(0)
#define Ava_PLATFORM_LIKELY(...)          (__VA_ARGS__)
#define Ava_PLATFORM_UNLIKELY(...)        (__VA_ARGS__)
#define Ava_PLATFORM_RESTRICT             __restrict
#define Ava_PLATFORM_DEBUGBREAK()         (__debugbreak())
