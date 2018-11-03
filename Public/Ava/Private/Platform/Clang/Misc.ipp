#define Ava_PLATFORM_PRAGMA(...)          _Pragma(Ava_PP_STR(__VA_ARGS__))
#define Ava_PLATFORM_CLANG_WARNING(...)   _Pragma(Ava_PP_STR(clang diagnostic __VA_ARGS__))
#define Ava_PLATFORM_FORCEINLINE          __inline__ __attribute__((always_inline))
#define Ava_PLATFORM_FORCENOINLINE        __attribute__((noinline))
#define Ava_PLATFORM_ASSUME(...)          ((__VA_ARGS__) ? (void)0 : __builtin_unreachable())
#define Ava_PLATFORM_UNREACHABLE()        (__builtin_unreachable())
#define Ava_PLATFORM_LIKELY(...)          (__builtin_expect(!!(__VA_ARGS__), 1))
#define Ava_PLATFORM_UNLIKELY(...)        (__builtin_expect(!!(__VA_ARGS__), 0))
#define Ava_PLATFORM_RESTRICT             __restrict__
#define Ava_PLATFORM_DEBUGBREAK()         (::Ava::Private::Platform_Clang::DebugBreak())

namespace Ava::Private::Platform_Clang {

[[maybe_unused]] static Ava_PLATFORM_FORCEINLINE void DebugBreak()
{
#if Ava_X86 || Ava_X64
	__asm__ __volatile__("int $0x03");
#endif
}

} // namespace Ava::Private::Platform_Clang
