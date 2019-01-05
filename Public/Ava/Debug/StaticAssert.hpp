#pragma once

#ifndef Ava_CONFIG_STATIC_ASSERT
#	define Ava_CONFIG_STATIC_ASSERT 2
#endif

#if Ava_CONFIG_STATIC_ASSERT >= 2
#	define Ava_StaticAssertSlow(...) static_assert(__VA_ARGS__)
#else
#	define Ava_StaticAssertSlow(...)
#endif
