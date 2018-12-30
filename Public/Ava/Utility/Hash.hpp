#pragma once

#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"
#include "Ava/Private/Utility/Hash.hpp"
#include "Ava/Utility/Default.hpp"
#include "Ava/Utility/Forward.hpp"
#include "Ava/Utility/Move.hpp"
#include "Ava/Utility/MurmurHash.hpp"

template<typename THash, typename T>
Ava_FORCEINLINE void Ava_Hash(THash& hash, const T& value)
{
	return Ava_Ext_Hash(hash, value);
}

namespace Ava {

typedef MurmurHash<Default::Value> DefaultHash;

struct IntegerHasher
{
	template<typename T>
	Ava_FORCEINLINE uword Hash(T value) const
	{
		if constexpr (sizeof(T) > sizeof(uword))
		{
			static_assert(sizeof(T) == 8);
			return (u32)(value >> 32) ^ (u32)value;
		}
		else return (uword)value;
	}
};

struct PointerHasher
{
	Ava_FORCEINLINE uword Hash(void* pointer) const
	{
		return (uword)pointer;
	}
};

template<typename TAlgorithm>
class BasicHasher : TAlgorithm
{
	class Interface
	{
	public:
		static constexpr uword BlockSize = TAlgorithm::BlockSize;

		Ava_FORCEINLINE void Mix(const void* data, uword size)
		{
			m_hash.Mix(data, size);
		}

		Ava_FORCEINLINE void MixWord(uword word)
		{
			m_hash.MixWord(word);
		}

		Ava_FORCEINLINE void MixBlock(const void* block)
		{
			m_hash.MixBlock(block);
		}

		template<typename T>
		Ava_FORCEINLINE void operator()(const T& value)
		{
			Ava_Hash(*this, value);
		}

	private:
		Ava_FORCEINLINE Interface(typename TAlgorithm::HashType hash)
			: m_hash(Move(hash))
		{
		}

		typename TAlgorithm::HashType m_hash;

		friend class BasicHasher<TAlgorithm>;
	};

public:
	template<typename... TArgs>
	explicit Ava_FORCEINLINE BasicHasher(TArgs&&... args)
		: TAlgorithm(Forward<TArgs>(args)...)
	{
	}

	template<typename T>
	uword Hash(const T& value) const
	{
		Interface hash(TAlgorithm::Initialize());
		Ava_Hash(hash, value);
		return TAlgorithm::Finalize(hash.m_hash);
	}
};

namespace Ext {

template<typename T>
struct SelectHasher
{
	typedef BasicHasher<DefaultHash> Type;
};

template<> struct SelectHasher<u8> { typedef IntegerHasher Type; };
template<> struct SelectHasher<i8> { typedef IntegerHasher Type; };
template<> struct SelectHasher<u16> { typedef IntegerHasher Type; };
template<> struct SelectHasher<i16> { typedef IntegerHasher Type; };
template<> struct SelectHasher<u32> { typedef IntegerHasher Type; };
template<> struct SelectHasher<i32> { typedef IntegerHasher Type; };
template<> struct SelectHasher<u64> { typedef IntegerHasher Type; };
template<> struct SelectHasher<i64> { typedef IntegerHasher Type; };

template<> struct SelectHasher<char> { typedef IntegerHasher Type; };
template<> struct SelectHasher<wchar_t> { typedef IntegerHasher Type; };

template<typename T>
struct SelectHasher<T*> { typedef PointerHasher Type; };

} // namespace Ext

template<typename T>
using SelectHasher = typename Ext::SelectHasher<T>::Type;

} // namespace Ava
