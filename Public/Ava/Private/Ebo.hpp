#pragma once

#include "Ava/Misc.hpp"
#include "Ava/Meta/Condition.hpp"
#include "Ava/Meta/TypeTraits.hpp"

namespace Ava::Private {

#ifdef __INTELLISENSE__
template<typename T1, typename T2>
using Ebo1 = T1;

template<typename T1, typename T2>
using Ebo2 = T2;
#else
template<typename T1, typename T2>
using Ebo1 = Condition<IsEmpty<T2>, T2, T1>;

template<typename T1, typename T2>
using Ebo2 = Condition<IsEmpty<T2>, T1, T2>;
#endif

} // namespace Ava::Private

#define Ava_EBO(...) \
	::Ava::Private::Ebo1<__VA_ARGS__>, \
	::Ava::Private::Ebo2<__VA_ARGS__>

#define Ava_EBOX(x, ...) \
	x ::Ava::Private::Ebo1<__VA_ARGS__>, \
	x ::Ava::Private::Ebo2<__VA_ARGS__>
