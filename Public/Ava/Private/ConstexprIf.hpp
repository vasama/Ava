#pragma once

#include "Ava/Misc.hpp"

#if Ava_MSVC

namespace Ava::Private::ConstexprIf_ {

template<bool TBool>
constexpr bool Bool = TBool;

} // namespace Ava::Private::ConstexprIf_

#	define Ava_IF_CONSTEXPR(...) if constexpr (::Ava::Private::ConstexprIf_::Bool<__VA_ARGS__>)
#else
#	define Ava_IF_CONSTEXPR(...) if constexpr (__VA_ARGS__)
#endif
