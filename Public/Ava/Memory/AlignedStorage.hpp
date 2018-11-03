#pragma once

#include "Ava/Math/Math.hpp"
#include "Ava/Types.hpp"

namespace Ava {

template<uword TSize, uword TAlign>
class AlignedStorage
{
	static_assert(Math::Constexpr::IsPowerOfTwo(TAlign));
	alignas(TAlign) char m_data[TSize];
};

} // namespace Ava
