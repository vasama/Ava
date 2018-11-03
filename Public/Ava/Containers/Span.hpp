#pragma once

#include "Ava/Debug/Assert.hpp"
#include "Ava/Memory/NoInit.hpp"
#include "Ava/Meta/EnableIf.hpp"
#include "Ava/Meta/Traits.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"

namespace Ava {

template<typename T>
struct Span
{
	typedef RemoveConst<T> ElementType;

	Ava_FORCEINLINE Span()
		: m_size(0)
	{
	}

	Ava_FORCEINLINE Span(T* data, iword size)
		: m_data(data), m_size(size)
	{
	}

	template<uword TSize>
	Ava_FORCEINLINE Span(T(&data)[TSize])
		: m_data(data), m_size((iword)TSize)
	{
	}

	template<typename TIn, uword TSize,
		typename = EnableIf<HasConv<TIn**, T* const*>>>
	Ava_FORCEINLINE Span(TIn(&data)[TSize])
		: m_data(data), m_size((iword)TSize)
	{
	}

	template<typename TIn,
		typename = EnableIf<HasConv<TIn**, T* const*>>>
	Ava_FORCEINLINE Span(Span<TIn> other)
		: m_data(other.m_data), m_size(other.m_size)
	{
	}

	Ava_FORCEINLINE Span(decltype(nullptr))
		: Span()
	{
	}

	Ava_FORCEINLINE Span(decltype(NoInit))
	{
	}


	Ava_FORCEINLINE T* Data() const
	{
		return m_data;
	}

	Ava_FORCEINLINE iword Size() const
	{
		return m_size;
	}


	Ava_FORCEINLINE T& First() const
	{
		Ava_Assert(m_size > 0);
		return m_data[0];
	}

	Ava_FORCEINLINE T& Last() const
	{
		Ava_Assert(m_size > 0);
		return m_data[m_size - 1];
	}

	Ava_FORCEINLINE T& operator[](iword index) const
	{
		Ava_Assert(index < m_size);
		return m_data[index];
	}

	Ava_FORCEINLINE Span Slice(iword index) const
	{
		Ava_Assert(m_size >= index);
		return Span(m_data + index, m_size - index);
	}

	Ava_FORCEINLINE Span Slice(iword index, iword count) const
	{
		Ava_Assert(m_size >= index + count);
		return Span(m_data + index, count);
	}

	Ava_FORCEINLINE void RemovePrefix(iword count)
	{
		Ava_Assert(count <= m_size);
		m_data += count;
		m_size -= count;
	}

	Ava_FORCEINLINE void RemoveSuffix(iword count)
	{
		Ava_Assert(count <= m_size);
		m_size -= count;
	}

	T* m_data;
	iword m_size;
};

template<typename T> constexpr bool IsTriviallyEquatable<Span<T>> = true;
template<typename T> constexpr bool IsZeroConstructible<Span<T>> = true;

template<typename TLhs, typename TRhs>
Ava_FORCEINLINE bool operator==(const Span<TLhs>& lhs, const Span<TRhs>& rhs)
{
	return lhs.m_data == rhs.m_data && lhs.m_size == rhs.m_size;
}

template<typename TLhs, typename TRhs>
Ava_FORCEINLINE bool operator!=(const Span<TLhs>& lhs, const Span<TRhs>& rhs)
{
	return lhs.m_data != rhs.m_data || lhs.m_size != rhs.m_size;
}

template<typename T>
Ava_FORCEINLINE T* begin(const Span<T>& span)
{
	return span.m_data;
}

template<typename T>
Ava_FORCEINLINE T* end(const Span<T>& span)
{
	return span.m_data + span.m_size;
}

} // namespace Ava
