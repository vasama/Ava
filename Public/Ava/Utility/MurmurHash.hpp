#pragma once

#include "Ava/Math/Bits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"
#include "Ava/Private/Export.hpp"
#include "Ava/Private/Utility/Hash.hpp"
#include "Ava/Utility/Default.hpp"

namespace Ava {

namespace Private::Utility_MurmurHash {

#if Ava_X86 || Ava_IDE
constexpr uword C1_x86 = 0xcc9e2d51;
constexpr uword C2_x86 = 0x1b873593;

Ava_FORCEINLINE uword MixBlock_x86(uword hash, uword block)
{
	block *= C1_x86;
	block = Math::Rotl(block, 15);
	block *= C2_x86;

	block ^= hash;
	block = Math::Rotl(block, 13);
	hash = block * 5 + 0xe6546b64;

	return hash;
}

Ava_FORCEINLINE uword MixFinal_x86(uword hash)
{
	hash ^= hash >> 16;
	hash *= 0x85ebca6b;
	hash ^= hash >> 13;
	hash *= 0xc2b2ae35;
	hash ^= hash >> 16;

	return hash;
}

Ava_API struct Hash_x86
{
	static constexpr uword BlockSize = 4;

	void Mix(const void* data, uword size);

	void MixWord(uword word)
	{
		m_hash = MixBlock_x86(m_hash, word);
		m_size += BlockSize;
	}

	Ava_FORCEINLINE void MixBlock(const void* block)
	{
		MixWord(*(uword*)block);
	}

	uword Finalize()
	{
		return MixFinal_x86(m_hash ^ m_size);
	}

	Ava_FORCEINLINE Hash_x86(uword seed)
		: m_hash(seed), m_size(0)
	{
	}

	uword m_hash;
	uword m_size;
} typedef Hash;
#endif

#if Ava_X64 || Ava_IDE
struct Block_x64
{
	uword m_hash1;
	uword m_hash2;
};

constexpr uword C1_x64 = 0x87c37b91114253d5;
constexpr uword C2_x64 = 0x4cf5ad432745937f;

Ava_FORCEINLINE Block_x64 MixBlock_x64(Block_x64 hash, uword block1, uword block2)
{
	block1 *= C1_x64;
	block1 = Math::Rotl(block1, 31);
	block1 *= C2_x64;
	hash.m_hash1 ^= block1;
	hash.m_hash1 = Math::Rotl(hash.m_hash1, 27);
	hash.m_hash1 += hash.m_hash2;
	hash.m_hash1 = hash.m_hash1 * 5 + 0x52dce729;

	block2 *= C2_x64;
	block2 = Math::Rotl(block2, 33);
	block2 *= C1_x64;
	hash.m_hash2 ^= block2;
	hash.m_hash2 = Math::Rotl(hash.m_hash2, 31);
	hash.m_hash2 += hash.m_hash1;
	hash.m_hash2 = hash.m_hash2 * 5 + 0x38495ab5;

	return hash;
}

Ava_FORCEINLINE uword MixFinal_x64(uword hash)
{
	hash ^= hash >> 33;
	hash *= 0xff51afd7ed558ccd;
	hash ^= hash >> 33;
	hash *= 0xc4ceb9fe1a85ec53;
	hash ^= hash >> 33;

	return hash;
}

Ava_API struct Hash_x64
{
	static constexpr uword BlockSize = 16;

	void Mix(const void* data, uword size);

	Ava_FORCEINLINE void MixWord(uword word)
	{
		MixBlock_Internal(0, word);
	}

	Ava_FORCEINLINE void MixBlock(const void* block)
	{
		MixBlock_Internal(((const uword*)block)[0], ((const uword*)block)[1]);
	}

	void MixBlock_Internal(uword block1, uword block2)
	{
		Block_x64 block{ m_hash1, m_hash2 };
		block = MixBlock_x64(block, block1, block2);
		m_hash1 = block.m_hash1; m_hash2 = block.m_hash2;
		m_size += BlockSize;
	}

	uword Finalize()
	{
		uword hash1 = m_hash1 ^ m_size;
		uword hash2 = m_hash2 ^ m_size;

		hash1 += hash2;
		hash2 += hash1;

		hash1 = MixFinal_x64(hash1);
		hash2 = MixFinal_x64(hash2);

		hash1 += hash2;
		hash2 += hash1;

		return hash2;
	}

	Ava_FORCEINLINE Hash_x64(uword seed)
		: m_hash1(seed), m_hash2(seed), m_size(0)
	{
	}

	uword m_hash1;
	uword m_hash2;
	uword m_size;
} typedef Hash;
#endif

enum class Dynamic { Value };

constexpr uword DefaultSeed = 0;

template<typename T, T TSeed>
class Algorithm
{
public:
	typedef Hash HashType;

	static constexpr uword BlockSize = Hash::BlockSize;

	Ava_FORCEINLINE Hash Initialize() const
	{
		return Hash(Seed);
	}

	Ava_FORCEINLINE uword Finalize(Hash& hash) const
	{
		return hash.Finalize();
	}

private:
	static constexpr uword Seed = (uword)TSeed;
};

template<>
class Algorithm<Default, Default::Value>
	: public Algorithm<uword, DefaultSeed>
{
};

template<>
class Algorithm<Dynamic, Dynamic::Value>
{
public:
	typedef Hash HashType;

	static constexpr uword BlockSize = Hash::BlockSize;

	Ava_FORCEINLINE Algorithm()
		: m_seed(DefaultSeed)
	{
	}

	explicit Ava_FORCEINLINE Algorithm(uword seed)
		: m_seed(seed)
	{
	}

	Ava_FORCEINLINE Hash Initialize() const
	{
		return Hash(m_seed);
	}

	Ava_FORCEINLINE uword Finalize(Hash& hash) const
	{
		return hash.Finalize();
	}

private:
	uword m_seed;

	friend Ava_FORCEINLINE bool operator==(
		const Algorithm<Dynamic, Dynamic::Value>& lhs,
		const Algorithm<Dynamic, Dynamic::Value>& rhs)
	{
		return lhs.m_seed == rhs.m_seed;
	}

	friend Ava_FORCEINLINE bool operator!=(
		const Algorithm<Dynamic, Dynamic::Value>& lhs,
		const Algorithm<Dynamic, Dynamic::Value>& rhs)
	{
		return lhs.m_seed != rhs.m_seed;
	}

};

template<typename T, T TSeed>
constexpr Ava_FORCEINLINE bool operator==(
	const Algorithm<T, TSeed>&, const Algorithm<T, TSeed>&)
{
	return true;
}

template<typename T, T TSeed>
constexpr Ava_FORCEINLINE bool operator!=(
	const Algorithm<T, TSeed>&, const Algorithm<T, TSeed>&)
{
	return false;
}

} // namespace Private::Utility_MurmurHash

template<auto TSeed = Private::Utility_MurmurHash::Dynamic::Value, typename T = decltype(TSeed)>
using MurmurHash = Private::Utility_MurmurHash::Algorithm<T, TSeed>;

} // namespace Ava
