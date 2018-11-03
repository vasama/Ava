#pragma once

#include "Ava/Misc.hpp"
#include "Ava/Private/Export.hpp"
#include "Ava/Types.hpp"

#ifndef Ava_CONFIG_ASSERT
#	define Ava_CONFIG_ASSERT 2
#endif

#define Ava_ASSERT_NOP(expr) Ava_UNUSED(expr)
#define Ava_ASSUME_NOP(expr) Ava_ASSUME(expr)
#define Ava_VERIFY_NOP(expr)           (expr)

#if Ava_CONFIG_ASSERT >= 1
namespace Ava::Private::Debug_Assert {
bool Ava_API Fail(const achar* expr, const achar* file,
	i32 line, const achar* fmt = nullptr, ...);
} // namespace Ava_Private::Debug_Assert

#	define Ava_ASSERT_ACTIVE 1

#	define Ava_ASSERT_FAIL(expr, ...) \
		(::Ava::Private::Debug_Assert::Fail(expr, __FILE__, \
			__LINE__, ##__VA_ARGS__) ? Ava_DEBUGBREAK() : ((void)0))

#	define Ava_ASSERT_IMPL(expr, ...) \
		(Ava_LIKELY(expr) ? true : (Ava_ASSERT_FAIL(#expr, ##__VA_ARGS__), false))

#	define Ava_Assert(expr, ...) ((void)Ava_ASSERT_IMPL(expr, ##__VA_ARGS__))
#	define Ava_Assume(expr, ...) ((void)Ava_ASSERT_IMPL(expr, ##__VA_ARGS__))
#	define Ava_Verify(expr, ...) ((bool)Ava_ASSERT_IMPL(expr, ##__VA_ARGS__))
#else
#	define Ava_ASSERT_ACTIVE 0

#	define Ava_Assert(expr, ...) Ava_ASSERT_NOP(expr)
#	define Ava_Assume(expr, ...) Ava_ASSUME_NOP(expr)
#	define Ava_Verify(expr, ...) Ava_VERIFY_NOP(expr)
#endif

#if Ava_CONFIG_ASSERT >= 2
#	define Ava_ASSERT_SLOW_ACTIVE 1

#	define Ava_AssertSlow(expr, ...) Ava_Assert(expr, ##__VA_ARGS__)
#	define Ava_AssumeSlow(expr, ...) Ava_Assume(expr, ##__VA_ARGS__)
#	define Ava_VerifySlow(expr, ...) Ava_Verify(expr, ##__VA_ARGS__)
#else
#	define Ava_ASSERT_SLOW_ACTIVE 0

#	define Ava_AssertSlow(expr, ...) Ava_ASSERT_NOP(expr)
#	define Ava_AssumeSlow(expr, ...) Ava_ASSUME_NOP(expr)
#	define Ava_VerifySlow(expr, ...) Ava_VERIFY_NOP(expr)
#endif
