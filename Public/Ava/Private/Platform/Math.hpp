#pragma once

#include "Ava/Preprocessor/Str.hpp"
#include "Ava/Private/Platform/Arch.hpp"
#include "Ava/Private/Platform/Compiler.hpp"

#if Ava_X86 || Ava_X64
#	include Ava_PP_STR(Ava/Private/Platform/Ava_COMPILER/x86_Intrin.ipp)
#endif
