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
class BasicStringSpan
{
	typedef Span<const TChar> SpanType;

public:
	typedef typename SpanType::ElementType CharType;

	Ava_FORCEINLINE BasicStringSpan()
	{
	}

	Ava_FORCEINLINE BasicStringSpan(Span<TChar> span)
		: m_span(span)
	{
	}

	Ava_FORCEINLINE BasicStringSpan(Span<const TChar> span)
		: m_span(span)
	{
	}

	Ava_FORCEINLINE BasicStringSpan(const TChar* data, iword size)
		: m_span(data, size)
	{
	}

	template<uword TSize>
	Ava_FORCEINLINE BasicStringSpan(const TChar(&string)[TSize])
		: m_span(string, BasicCString<TChar>::Size(string, TSize))
	{
	}

	Ava_FORCEINLINE BasicStringSpan(decltype(nullptr))
		: m_span(nullptr)
	{
	}

	Ava_FORCEINLINE BasicStringSpan(decltype(NoInit))
		: m_span(NoInit)
	{
	}


	Ava_FORCEINLINE const TChar* Data() const
	{
		return m_span.Data();
	}

	Ava_FORCEINLINE iword Size() const
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


	Ava_FORCEINLINE BasicStringSpan Slice(iword index)
	{
		return BasicStringSpan(m_span.Slice(index));
	}

	Ava_FORCEINLINE BasicStringSpan Slice(iword index, iword count)
	{
		return BasicStringSpan(m_span.Slice(index, count));
	}

	Ava_FORCEINLINE void RemovePrefix(iword count)
	{
		m_span.RemovePrefix(count);
	}

	Ava_FORCEINLINE void RemoveSuffix(iword count)
	{
		m_span.RemoveSuffix(count);
	}


	static Ava_FORCEINLINE BasicStringSpan FromCString(const TChar* cstr)
	{
		return BasicStringSpan(cstr, BasicCString<TChar>::Size(cstr));
	}

	SpanType m_span;
};

template<typename T>
Ava_FORCEINLINE iword Ava_Ext_Size(const BasicStringSpan<T>& span)
{
	return span.m_span.m_size;
}

template<typename T>
bool operator==(const BasicStringSpan<T>& lhs, const BasicStringSpan<T>& rhs)
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
Ava_FORCEINLINE bool operator!=(const BasicStringSpan<T>& lhs, const BasicStringSpan<T>& rhs)
{
	return !(lhs == rhs);
}

template<typename T>
Ava_FORCEINLINE const T* begin(const BasicStringSpan<T>& span)
{
	return span.m_span.m_data;
}

template<typename T>
Ava_FORCEINLINE const T* end(const BasicStringSpan<T>& span)
{
	return span.m_span.m_data + span.m_span.m_size;
}

template<typename T> constexpr bool IsZeroConstructible<BasicStringSpan<T>> = true;

typedef BasicStringSpan<char> StringSpan;

} // namespace Ava
