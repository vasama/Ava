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
	static iword Size(const CharType* cstr, iword maxSize);

	static i32 Compare(const CharType* cstr1, const CharType* cstr2);
	static i32 Compare(const CharType* cstr1, const CharType* cstr2, iword maxSize);

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

template<>
Ava_FORCEINLINE i32 BasicCString<char>::Compare(const char* cstr1, const char* cstr2)
{
	return ::strcmp(cstr1, cstr2);
}

template<>
Ava_FORCEINLINE i32 BasicCString<char>::Compare(const char* cstr1, const char* cstr2, iword maxSize)
{
	return ::strncmp(cstr1, cstr2, maxSize);
}

template<>
Ava_FORCEINLINE i32 BasicCString<wchar_t>::Compare(const wchar_t* cstr1, const wchar_t* cstr2)
{
	return ::wcscmp(cstr1, cstr2);
}

template<>
Ava_FORCEINLINE i32 BasicCString<wchar_t>::Compare(const wchar_t* cstr1, const wchar_t* cstr2, iword maxSize)
{
	return ::wcsncmp(cstr1, cstr2, maxSize);
}

typedef BasicCString<char> CString;

} // namespace Ava
