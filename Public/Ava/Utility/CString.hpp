#pragma once

#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"

#include <string.h>
#include <wchar.h>

namespace Ava {

template<typename TChar>
struct BasicCString
{
	typedef TChar CharType;

	static iword Size(const CharType* cstr);
	static iword Size(const CharType* cstr, iword maxLength);

	BasicCString() = delete;
};

template<>
Ava_FORCEINLINE iword BasicCString<char>::Size(const char* cstr)
{
	return ::strlen(cstr);
}

template<>
Ava_FORCEINLINE iword BasicCString<char>::Size(const char* cstr, iword maxLength)
{
	return ::strnlen(cstr, maxLength);
}

template<>
Ava_FORCEINLINE iword BasicCString<wchar_t>::Size(const wchar_t* cstr)
{
	return ::wcslen(cstr);
}

template<>
Ava_FORCEINLINE iword BasicCString<wchar_t>::Size(const wchar_t* cstr, iword maxLength)
{
	return ::wcsnlen(cstr, maxLength);
}

typedef BasicCString<char> CString;

} // namespace Ava
