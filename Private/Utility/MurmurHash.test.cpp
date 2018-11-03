#include "Ava/Utility/MurmurHash.hpp"

#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"
#include "Ava/Utility/Hash.hpp"

#include "doctest.h"

using namespace Ava;

namespace {

TEST_SUITE_BEGIN("Utility/MurmurHash");

#if Ava_X86 || Ava_IDE
TEST_CASE("Consistency (x86)")
{
	u32 array[] = {
		0xECB00085, 0x7141F1FD, 0xE3DFB6E0, 0x6B9701CE,
		0x702A813A, 0xB54B5801, 0xB2CF7EE3, 0x9B99B014,
		0xBEB1714C, 0xE2BAA2C4, 0x18A21599, 0xFB85FD84,
		0xBCAE254D, 0xCCCDD8A1, 0x58FFD2A2, 0xC30E614A,
		0x2FA222BA, 0x26B5EBB5, 0x87BD2212, 0x35BF052D,
		0xD2FA26ED, 0x746247F1, 0xD9CFF4E3, 0xE8000009,
		0x41CB6A59, 0x63425D07, 0xC26FC60F, 0xAAC56C99,
		0xF9A5D0C9, 0xE773D5CC, 0x5A30E156, 0xC008CD87,
	};

	BasicHasher<MurmurHash<0>> hasher;
	uword hash = hasher.Hash(array);

	CHECK(hash == 0xc1995dd3);
}
#endif

#if Ava_X64 || Ava_IDE
TEST_CASE("Consistency (x64)")
{
	u32 array[] = {
		0xECB00085, 0x7141F1FD, 0xE3DFB6E0, 0x6B9701CE,
		0x702A813A, 0xB54B5801, 0xB2CF7EE3, 0x9B99B014,
		0xBEB1714C, 0xE2BAA2C4, 0x18A21599, 0xFB85FD84,
		0xBCAE254D, 0xCCCDD8A1, 0x58FFD2A2, 0xC30E614A,
		0x2FA222BA, 0x26B5EBB5, 0x87BD2212, 0x35BF052D,
		0xD2FA26ED, 0x746247F1, 0xD9CFF4E3, 0xE8000009,
		0x41CB6A59, 0x63425D07, 0xC26FC60F, 0xAAC56C99,
		0xF9A5D0C9, 0xE773D5CC, 0x5A30E156, 0xC008CD87,
	};

	BasicHasher<MurmurHash<0>> hasher;
	uword hash = hasher.Hash(array);

	CHECK(hash == 0xe8e353a641afb232);
}
#endif

TEST_SUITE_END();

} // namespace
