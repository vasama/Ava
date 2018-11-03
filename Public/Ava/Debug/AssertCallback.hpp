#pragma once

#include "Ava/Containers/StringSpan.hpp"

namespace Ava::Debug {

struct AssertInfo
{
	AStringSpan Expr;
	AStringSpan File;
	i32 Line;

	AStringSpan Message;
};

enum class AssertAction
{
	Abort = 0,
	Break = 1,
	Continue = 2,

	Default = Abort,
};

Ava::Debug::AssertAction AssertCallback(Ava::Debug::AssertInfo* info);

} // namespace Ava::Debug
