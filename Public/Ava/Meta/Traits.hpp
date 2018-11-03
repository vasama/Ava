#pragma once

#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"

#include <type_traits>

namespace Ava {

template<typename T>
constexpr bool IsTriviallyConstructible = std::is_trivially_constructible_v<T>;

template<typename T>
constexpr bool IsTriviallyDestructible = std::is_trivially_destructible_v<T>;

template<typename T>
constexpr bool IsTriviallyCopyable = std::is_trivially_copyable_v<T>;

template<typename T>
constexpr bool IsTriviallyRelocatable = IsTriviallyCopyable<T> && IsTriviallyDestructible<T>;

template<typename T>
constexpr bool IsZeroConstructible = IsTriviallyConstructible<T>;

template<typename T>
constexpr bool IsTriviallyEquatable = std::is_integral_v<T>;

template<typename T>
constexpr bool IsTriviallyEquatable<T*> = true;

template<typename T>
constexpr bool IsTriviallyHashable = std::is_integral_v<T>;

template<typename T>
constexpr bool IsTriviallyHashable<T*> = true;

template<typename T>
constexpr bool IsContiguousIterator = false;

template<typename T>
constexpr bool IsContiguousIterator<T*> = true;

template<typename T>
struct TombstoneTraits
{
	static constexpr uword Count = 0;

	static constexpr Ava_FORCEINLINE uword GetIndex(const T*)
	{
		return (uword)-1;
	}

	static void SetIndex(T* bits, uword index) = delete;
};

} // namespace Ava
