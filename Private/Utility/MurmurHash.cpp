#include "Ava/Utility/MurmurHash.hpp"

using namespace Ava;

#if Ava_X86 || Ava_IDE
void Ava::Private::Utility_MurmurHash::Hash_x86
	::Mix(const void* data, uword size)
{
	uword hash = m_hash;

	const uword* first = (const uword*)data;
	const uword* last = first + size / sizeof(uword);

	for (; first != last; ++first)
		hash = MixBlock_x86(hash, *first);

	const byte* tail = (const byte*)last;

	uword temp = 0;
	switch (size % sizeof(uword))
	{
	case 3: temp ^= (uword)tail[2] << 16;
	case 2: temp ^= (uword)tail[1] << 8;
	case 1: temp ^= (uword)tail[0] << 0;
		hash = MixBlock_x86(hash, temp);
	}

	m_hash = hash;
	m_size += size;
}
#endif

#if Ava_X64 || Ava_IDE
void Ava::Private::Utility_MurmurHash::Hash_x64
	::Mix(const void* data, uword size)
{
	Block_x64 hash{ m_hash1, m_hash2 };

	const uword* first = (const uword*)data;
	const uword* last = first + size / sizeof(uword);

	for (; first != last; first += 2)
		hash = MixBlock_x64(hash, first[0], first[1]);

	const byte* tail = (const byte*)last;

	uword temp1 = 0;
	uword temp2 = 0;
	switch (size % (sizeof(uword) * 2))
	{
	case 15: temp2 ^= ((uword)tail[14]) << 48;
	case 14: temp2 ^= ((uword)tail[13]) << 40;
	case 13: temp2 ^= ((uword)tail[12]) << 32;
	case 12: temp2 ^= ((uword)tail[11]) << 24;
	case 11: temp2 ^= ((uword)tail[10]) << 16;
	case 10: temp2 ^= ((uword)tail[9]) << 8;
	case  9: temp2 ^= ((uword)tail[8]) << 0;
		temp2 *= C2_x64;
		temp2 = Math::Rotl(temp2, 33);
		temp2 *= C1_x64;
		hash.m_hash2 ^= temp2;

	case  8: temp1 ^= ((uword)tail[7]) << 56;
	case  7: temp1 ^= ((uword)tail[6]) << 48;
	case  6: temp1 ^= ((uword)tail[5]) << 40;
	case  5: temp1 ^= ((uword)tail[4]) << 32;
	case  4: temp1 ^= ((uword)tail[3]) << 24;
	case  3: temp1 ^= ((uword)tail[2]) << 16;
	case  2: temp1 ^= ((uword)tail[1]) << 8;
	case  1: temp1 ^= ((uword)tail[0]) << 0;
		temp1 *= C1_x64;
		temp1 = Math::Rotl(temp1, 31);
		temp1 *= C2_x64;
		hash.m_hash1 ^= temp1;
	}

	m_hash1 = hash.m_hash1;
	m_hash2 = hash.m_hash2;
	m_size += size;
}
#endif
