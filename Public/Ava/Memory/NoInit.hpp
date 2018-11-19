#pragma once

namespace Ava {

namespace Private::Memory_ {

enum class NoInit {};

} // namespace Private::Memory_

constexpr Private::Memory_::NoInit NoInit = {};

} // namespace Ava
