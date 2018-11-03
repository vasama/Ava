#if Ava_X86 || Ava_X64
#	include "Ava/Private/Platform/x86/popcnt.ipp"
#	include "Ava/Private/Platform/x86/zcnt.ipp"
#else
#	include "Ava/Private/Platform/Generic/popcnt.ipp"
#	include "Ava/Private/Platform/Generic/zcnt.ipp"
#endif

#include "Ava/Private/Platform/Generic/rot.ipp"
