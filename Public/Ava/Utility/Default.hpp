#pragma once

#include "Ava/Meta/Condition.hpp"
#include "Ava/Meta/TypeTraits.hpp"

namespace Ava {

enum class Default { Value };

template<typename T, typename TDefault>
using SelectDefault = Condition<IsSame<T, Default>, TDefault, T>;

} // namespace Ava
