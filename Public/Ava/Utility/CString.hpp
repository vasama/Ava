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
Ava_FORCEINLINE iword CString<achar>::Length(const achar* cstr)
{
	return ::strlen(cstr);
}

template<>
Ava_FORCEINLINE iword CString<achar>::Length(const achar* cstr, iword maxLength)
{
	return ::strnlen(cstr, maxLength);
}

template<>
Ava_FORCEINLINE iword CString<wchar>::Length(const wchar* cstr)
{
	return ::wcslen(cstr);
}

template<>
Ava_FORCEINLINE iword CString<wchar>::Length(const wchar* cstr, iword maxLength)
{
	return ::wcsnlen(cstr, maxLength);
}

typedef CString<achar> ACString;
typedef CString<wchar> WCString;

} // namespace Ava
