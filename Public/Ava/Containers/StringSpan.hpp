#pragma once

#include "Ava/Containers/Span.hpp"
#include "Ava/Memory/Memory.hpp"
#include "Ava/Memory/NoInit.hpp"
#include "Ava/Meta/Identity.hpp"
#include "Ava/Meta/Traits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"
#include "Ava/Utility/CString.hpp"
#include "Ava/Utility/Key.hpp"

namespace Ava {

template<typename TChar>
class BasicStringSpan
{
	typedef Span<const TChar> SpanType;

public:
	typedef TChar CharType;

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

	Ava_FORCEINLINE BasicStringSpan(const TChar* begin, const TChar* end)
		: m_span(begin, end)
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


	Ava_FORCEINLINE const TChar* Begin() const
	{
		return m_span.Begin();
	}

	Ava_FORCEINLINE const TChar* End() const
	{
		return m_span.End();
	}

	Ava_FORCEINLINE const TChar* Data() const
	{
		return m_span.Data();
	}

	Ava_FORCEINLINE iword Size() const
	{
		return m_span.Size();
	}

	Ava_FORCEINLINE uword ByteSize() const
	{
		return m_span.ByteSize();
	}

	Ava_FORCEINLINE bool IsEmpty() const
	{
		return m_span.IsEmpty();
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
	return span.m_span.Size();
}

template<typename THash, typename T>
Ava_FORCEINLINE void Ava_Ext_Hash(THash& hash, const BasicStringSpan<T>& span)
{
	hash.Mix((void*)span.Data(), span.ByteSize());
}

#if Ava_MSVC && !defined(__INTELLISENSE__)
#	define Ava_WORKAROUND(x) , int = x
#else
#	define Ava_WORKAROUND(x)
#endif

template<typename T>
Ava_FORCEINLINE bool operator==(const BasicStringSpan<T>& lhs, const BasicStringSpan<T>& rhs)
{
	uword byteSize = lhs.ByteSize();
	if (byteSize != rhs.ByteSize())
		return false;

	return Memory::Compare(lhs.Data(), rhs.Data(), byteSize) == 0;
}

template<typename T Ava_WORKAROUND(1)>
Ava_FORCEINLINE bool operator==(const BasicStringSpan<T>& lhs, const Identity<BasicStringSpan<T>>& rhs)
{
	return lhs == BasicStringSpan<T>(rhs);
}

template<typename T Ava_WORKAROUND(2)>
Ava_FORCEINLINE bool operator==(const Identity<BasicStringSpan<T>>& lhs, const BasicStringSpan<T>& rhs)
{
	return BasicStringSpan<T>(lhs) == rhs;
}

template<typename T>
Ava_FORCEINLINE bool operator!=(const BasicStringSpan<T>& lhs, const BasicStringSpan<T>& rhs)
{
	uword byteSize = lhs.ByteSize();
	if (byteSize != rhs.ByteSize())
		return true;

	return Memory::Compare(lhs.Data(), rhs.Data(), byteSize) != 0;
}

template<typename T Ava_WORKAROUND(1)>
Ava_FORCEINLINE bool operator!=(const BasicStringSpan<T>& lhs, const Identity<BasicStringSpan<T>>& rhs)
{
	return lhs != BasicStringSpan<T>(rhs);
}

template<typename T Ava_WORKAROUND(2)>
Ava_FORCEINLINE bool operator!=(const Identity<BasicStringSpan<T>>& lhs, const BasicStringSpan<T>& rhs)
{
	return BasicStringSpan<T>(lhs) != rhs;
}

#undef Ava_WORKAROUND

template<typename T>
Ava_FORCEINLINE const T* begin(const BasicStringSpan<T>& span)
{
	return span.m_span.Begin();
}

template<typename T>
Ava_FORCEINLINE const T* end(const BasicStringSpan<T>& span)
{
	return span.m_span.End();
}

template<typename T> constexpr bool IsZeroConstructible<BasicStringSpan<T>> = true;

typedef BasicStringSpan<char> StringSpan;

namespace Ext {

template<typename T>
struct SelectKeyClass<BasicStringSpan<T>>
{
	typedef const T Type[1];
};

} // namespace Ext

} // namespace Ava
