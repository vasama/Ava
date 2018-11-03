#include "Ava/Memory/Memory.hpp"

#include "Ava/Private/Platform/Os.hpp"

#if Ava_WINDOWS
#	include <Windows.h>
#endif

using namespace Ava;

void* Ava::Memory::ZeroSecure(void* dst, uword size)
{
#if Ava_WINDOWS
	return SecureZeroMemory(dst, size);
#else
	volatile byte* data = (volatile byte*)dst;

	for (uword i = 0; i < size; ++i)
		data[i] = 0;

	return dst;
#endif
}
