#include "Ava/Memory/Memory.hpp"

using namespace Ava;

static_assert(Memory::Constexpr::CalculateAlignment(0) == 1);
static_assert(Memory::Constexpr::CalculateAlignment(1) == 1);
static_assert(Memory::Constexpr::CalculateAlignment(3) == 4);
static_assert(Memory::Constexpr::CalculateAlignment(4) == 4);
static_assert(Memory::Constexpr::CalculateAlignment(5) == 8);
