#pragma once

#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"

#include <string.h>
#include <wchar.h>

namespace Ava {

template<typename TChar>
struct CString
{
	typedef TChar CharType;

	static iword Length(const CharType* cstr);
	static iword Length(const CharType* cstr, iword maxLength);

	CString() = delete;
};

template<>
Ava_FORCEINLINE iword CString<char>::Length(const char* cstr)
{
	return ::strlen(cstr);
}

template<>
Ava_FORCEINLINE iword CString<char>::Length(const char* cstr, iword maxLength)
{
	return ::strnlen(cstr, maxLength);
}

template<>
Ava_FORCEINLINE iword CString<wchar_t>::Length(const wchar_t* cstr)
{
	return ::wcslen(cstr);
}

template<>
Ava_FORCEINLINE iword CString<wchar_t>::Length(const wchar_t* cstr, iword maxLength)
{
	return ::wcsnlen(cstr, maxLength);
}

} // namespace Ava
