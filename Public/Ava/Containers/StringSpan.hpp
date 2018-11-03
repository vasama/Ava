#pragma once

#include "Ava/Containers/Span.hpp"
#include "Ava/Memory/Memory.hpp"
#include "Ava/Memory/NoInit.hpp"
#include "Ava/Meta/Traits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"
#include "Ava/Utility/CString.hpp"

namespace Ava {

template<typename TChar>
class StringSpan
{
	typedef Span<const TChar> SpanType;

public:
	typedef typename SpanType::ElementType CharType;

	Ava_FORCEINLINE StringSpan()
	{
	}

	Ava_FORCEINLINE StringSpan(Span<TChar> span)
		: m_span(span)
	{
	}

	Ava_FORCEINLINE StringSpan(Span<const TChar> span)
		: m_span(span)
	{
	}

	Ava_FORCEINLINE StringSpan(const TChar* data, iword size)
		: m_span(data, size)
	{
	}

	template<uword TSize>
	Ava_FORCEINLINE StringSpan(const TChar(&string)[TSize])
		: m_span(string, CString<TChar>::Length(string, TSize))
	{
	}

	Ava_FORCEINLINE StringSpan(decltype(nullptr))
		: m_span(nullptr)
	{
	}

	Ava_FORCEINLINE StringSpan(decltype(NoInit))
		: m_span(NoInit)
	{
	}


	Ava_FORCEINLINE const TChar* Data() const
	{
		return m_span.Data();
	}

	Ava_FORCEINLINE iword Length() const
	{
		return m_span.Size();
	}


	Ava_FORCEINLINE TChar First() const
	{
		return m_span.First();
	}

	Ava_FORCEINLINE TChar Last() const
	{
		return m_span.Last();
	}

	Ava_FORCEINLINE TChar operator[](iword index) const
	{
		return m_span[index];
	}


	Ava_FORCEINLINE StringSpan Slice(iword index)
	{
		return StringSpan(m_span.Slice(index));
	}

	Ava_FORCEINLINE StringSpan Slice(iword index, iword count)
	{
		return StringSpan(m_span.Slice(index, count));
	}

	Ava_FORCEINLINE void RemovePrefix(iword count)
	{
		m_span.RemovePrefix(count);
	}

	Ava_FORCEINLINE void RemoveSuffix(iword count)
	{
		m_span.RemoveSuffix(count);
	}


	static Ava_FORCEINLINE StringSpan FromCString(const TChar* cstr)
	{
		return StringSpan(cstr, CString<TChar>::Length(cstr));
	}

	SpanType m_span;
};

template<typename T>
Ava_FORCEINLINE iword Ava_Ext_Size(const StringSpan<T>& span)
{
	return span.m_span.m_size;
}

template<typename T>
bool operator==(const StringSpan<T>& lhs, const StringSpan<T>& rhs)
{
	Span<const T> ls = lhs.m_span;
	Span<const T> rs = rhs.m_span;

	if (ls.m_size != rs.m_size)
		return false;

	if (ls.m_data == rs.m_data)
		return true;

	return Memory::Compare(ls.m_data, rs.m_data,
		ls.m_size * sizeof(T)) == 0;
}

template<typename T>
Ava_FORCEINLINE bool operator!=(const StringSpan<T>& lhs, const StringSpan<T>& rhs)
{
	return !(lhs == rhs);
}

template<typename T>
Ava_FORCEINLINE const T* begin(const StringSpan<T>& span)
{
	return span.m_span.m_data;
}

template<typename T>
Ava_FORCEINLINE const T* end(const StringSpan<T>& span)
{
	return span.m_span.m_data + span.m_span.m_size;
}

template<typename T> constexpr bool IsZeroConstructible<StringSpan<T>> = true;

typedef StringSpan<achar> AStringSpan;
typedef StringSpan<wchar> WStringSpan;

} // namespace Ava
