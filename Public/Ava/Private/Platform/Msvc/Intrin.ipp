#if Ava_X86 || Ava_X64
#	include <intrin.h>
#	include "Ava/Platform/x86/Intrin_popcnt.ipp"
#	include "Ava/Platform/x86/Intrin_zcnt.h"
#else
#	include "Ava/Platform/Generic/Intrin_popcnt.ipp"
#	include "Ava/Platform/Generic/Intrin_zcnt.ipp"
#endif

#include "Ava/Platform/Generic/Intrin_rot.ipp"
