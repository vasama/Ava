#pragma once

#include "Ava/Containers/KeyValuePair.hpp"
#include "Ava/Misc.hpp"

namespace Ava::Private::Containers {

struct MapTraits
{
	template<typename T>
	using SelectKeyType = typename T::KeyType;

	template<typename TKey, typename TValue>
	static Ava_FORCEINLINE TKey& SelectKey(KeyValuePair<TKey, TValue>& pair)
	{
		return pair.Key;
	}
};

} // namespace Ava::Private::Containers
