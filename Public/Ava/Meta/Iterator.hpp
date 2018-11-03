#pragma once

#include "Ava/Meta/Constant.hpp"
#include "Ava/Meta/ValueOf.hpp"
#include "Ava/Utility/PriorityTag.hpp"

#include <type_traits>

namespace Ava {

namespace IteratorCategory {

enum : u32
{
	None = 0,

	Forward = 1,
	Bidirectional = 2,
	RandomAccess = 3,
};

} // namespace IteratorCategory

namespace Private::Meta_Iterator {

template<typename T>
auto Test(PriorityTag<3>) -> decltype(ValueOf<T>+0,
	Constant<IteratorCategory::RandomAccess>());

template<typename T>
auto Test(PriorityTag<2>) -> decltype(--ValueOf<T>,
	Constant<IteratorCategory::Bidirectional>());

template<typename T>
auto Test(PriorityTag<1>) -> decltype(++ValueOf<T>,
	Constant<IteratorCategory::Forward>());

template<typename T>
auto Test(PriorityTag<0>) -> Constant<IteratorCategory::None>;

} // namespace Private::Meta_Iterator

namespace IteratorCategory {

template<typename T>
static constexpr decltype(IteratorCategory::None) Get =
	decltype(Private::Meta_Iterator::Test<T>(PriorityTag<4>()))::Value;

} // namespace IteratorCategory

template<typename T>
using IteratorValue = std::remove_reference_t<decltype(*ValueOf<T>)>;

} // namespace Ava
