#pragma once

#include "Ava/Types.hpp"

#define Ava_PTRADD(p, o) ((decltype(p))((::Ava::byte*)(p) + (::Ava::uword)(o)))
#define Ava_PTRSUB(a, b) ((::Ava::iword)((::Ava::byte*)(a) - (::Ava::byte*)(b)))
