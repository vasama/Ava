#pragma once

#include "Ava/Misc.hpp"
#include "Ava/Utility/Move.hpp"

namespace Ava {

template<typename TKey, typename TValue>
struct KeyValuePair
{
	typedef TKey KeyType;
	typedef TValue ValueType;

	TKey Key;
	TValue Value;

	explicit Ava_FORCEINLINE KeyValuePair(TKey key)
		: Key(Move(key))
	{
	}

	Ava_FORCEINLINE KeyValuePair(TKey key, TValue value)
		: Key(Move(key)), Value(Move(value))
	{
	}
};

} // namespace Ava
