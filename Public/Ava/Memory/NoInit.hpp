#pragma once

namespace Ava {

namespace Private::Memory {
enum class NoInit {};
} // namespace Private::Memory

constexpr Private::Memory::NoInit NoInit = {};

} // namespace Ava
