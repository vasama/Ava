#pragma once

#include "Ava/Debug/Assert.hpp"
#include "Ava/Memory/NoInit.hpp"
#include "Ava/Meta/EnableIf.hpp"
#include "Ava/Meta/Traits.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Private/PointerMath.hpp"
#include "Ava/Types.hpp"

namespace Ava {

template<typename T>
class Span;

namespace Private::Containers_Span {

template<typename T>
class Core
{
public:
	typedef T ElementType;

	Ava_FORCEINLINE Core()
		: m_f(nullptr), m_l(nullptr)
	{
	}

	Ava_FORCEINLINE Core(T* f, T* l)
		: m_f(f), m_l(l)
	{
	}

	Ava_FORCEINLINE Core(decltype(nullptr))
		: m_f(nullptr), m_l(nullptr)
	{
	}

	Ava_FORCEINLINE Core(decltype(NoInit))
	{
	}

	template<typename TIn>
	Core(Span<TIn>) = delete;

	template<typename TIn, uword TSize>
	Core(TIn(&)[TSize]) = delete;

	template<typename TIn>
	Core(TIn*, TIn*) = delete;


	Ava_FORCEINLINE uword ByteSize() const
	{
		return Ava_PTRSUB(m_l, m_f);
	}

	Ava_FORCEINLINE bool IsEmpty() const
	{
		return m_f == m_l;
	}


	Ava_FORCEINLINE T* Data() const
	{
		return m_f;
	}

	Ava_FORCEINLINE T* Begin() const
	{
		return m_f;
	}

	Ava_FORCEINLINE T* End() const
	{
		return m_l;
	}

protected:
	T* m_f;
	T* m_l;

	template<typename>
	friend class Span;

	template<typename>
	friend class ConstLayer;

	template<typename>
	friend class ValueLayer;

	template<typename>
	friend class VoidLayer;
};

template<typename T>
class ConstLayer : public Core<T>
{
	typedef Core<T> Base;

public:
	using Base::Base;
};

template<typename T>
class ConstLayer<const T> : public Core<const T>
{
	typedef Core<const T> Base;

public:
	using Base::Base;

	Ava_FORCEINLINE ConstLayer(T* f, T* l)
		: Base(static_cast<const T*>(f), static_cast<const T*>(l))
	{
	}

	Ava_FORCEINLINE ConstLayer(const T* f, T* l)
		: Base(f, static_cast<const T*>(l))
	{
	}

	Ava_FORCEINLINE ConstLayer(T* f, const T* l)
		: Base(static_cast<const T*>(f), l)
	{
	}

	Ava_FORCEINLINE ConstLayer(Span<T> span)
		: Base(static_cast<const T*>(span.m_f),
			static_cast<const T*>(span.m_l))
	{
	}
};

template<typename T>
class ConstValueLayer : public ConstLayer<T>
{
	typedef ConstLayer<T> Base;

public:
	using Base::Base;
};

template<typename T>
class ConstValueLayer<const T> : public ConstLayer<const T>
{
	typedef ConstLayer<const T> Base;

public:
	using Base::Base;

	template<uword TSize>
	Ava_FORCEINLINE ConstValueLayer(T(&array)[TSize])
		: Base(static_cast<const T*>(array),
			static_cast<const T*>(array) + TSize)
	{
	}
};

template<typename T>
class ValueLayer : public ConstValueLayer<T>
{
	typedef ConstValueLayer<T> Base;

public:
	using Base::Base;
};

template<typename T>
class ValueLayer<T* const>
	: public ConstValueLayer<T* const>
{
	typedef ConstValueLayer<T* const> Base;

public:
	using Base::Base;
};

template<typename T>
class ValueLayer<T const* const>
	: public ConstValueLayer<T const* const>
{
	typedef ConstValueLayer<T const* const> Base;

public:
	using Base::Base;

	Ava_FORCEINLINE ValueLayer(Span<T*> span)
		: Base(static_cast<T const* const*>(span.m_f),
			static_cast<T const* const*>(span.m_l))
	{
	}

	Ava_FORCEINLINE ValueLayer(Span<T* const> span)
		: Base(static_cast<T const* const*>(span.m_f),
			static_cast<T const* const*>(span.m_l))
	{
	}

	template<uword TSize>
	Ava_FORCEINLINE ValueLayer(T*(&array)[TSize])
		: Base(static_cast<T const* const*>(array),
			static_cast<T const* const*>(array) + TSize)
	{
	}

	template<uword TSize>
	Ava_FORCEINLINE ValueLayer(T* const(&array)[TSize])
		: Base(static_cast<T const* const*>(array),
			static_cast<T const* const*>(array) + TSize)
	{
	}
};

template<typename T>
Ava_FORCEINLINE T* begin(const ValueLayer<T>& span)
{
	return span.Begin();
}

template<typename T>
Ava_FORCEINLINE T* end(const ValueLayer<T>& span)
{
	return span.End();
}

template<typename T>
class VoidLayer : public ConstLayer<T>
{
	typedef ConstLayer<T> Base;

public:
	using Base::Base;

	Ava_FORCEINLINE VoidLayer(T* data, uword size)
		: Base(data, Ava_PTRADD(data, size))
	{
	}

	template<typename TIn>
	Ava_FORCEINLINE VoidLayer(Span<TIn> span)
		: Base(static_cast<T*>(span.m_f), static_cast<T*>(span.m_l))
	{
	}

	template<typename TIn, uword TSize>
	Ava_FORCEINLINE VoidLayer(TIn(&array)[TSize])
		: Base(static_cast<T*>(array), static_cast<T*>(array + TSize))
	{
	}

	template<typename TIn>
	Ava_FORCEINLINE VoidLayer(TIn* f, TIn* l)
		: Base(static_cast<T*>(f), static_cast<T*>(l))
	{
	}

	template<typename TIn>
	VoidLayer(TIn*, uword) = delete;
};

} // namespace Private::Containers_Span

template<typename T>
class Span : public Private::Containers_Span::ValueLayer<T>
{
	typedef Private::Containers_Span::ValueLayer<T> Base;

public:
	using Base::Base;

#ifdef __INTELLISENSE__
	Span();
#endif

	Ava_FORCEINLINE Span(T* f, T* l)
		: Base(f, l)
	{
	}

	Ava_FORCEINLINE Span(T* data, iword size)
		: Base(data, data + size)
	{
	}

	template<uword TSize>
	Ava_FORCEINLINE Span(T(&array)[TSize])
		: Base(array, array + TSize)
	{
	}


	Ava_FORCEINLINE iword Size() const
	{
		return Base::m_l - Base::m_f;
	}


	Ava_FORCEINLINE T& First() const
	{
		Ava_Assert(Base::m_f != Base::m_l);
		return Base::m_f[0];
	}

	Ava_FORCEINLINE T& Last() const
	{
		Ava_Assert(Base::m_f != Base::m_l);
		return Base::m_l[-1];
	}

	Ava_FORCEINLINE T& operator[](iword index) const
	{
		Ava_Assert(CheckIndex(index));
		return Base::m_f[index];
	}


	Ava_FORCEINLINE Span<T> Slice(iword index) const
	{
		Ava_Assert(CheckIndex(index));
		return Span<T>(Base::m_f + index, Base::m_l);
	}

	Ava_FORCEINLINE Span<T> Slice(iword index, iword count) const
	{
		Ava_Assert(CheckRange(index, count));

		T* f = Base::m_f + index;
		return Span<T>(f, f + count);
	}


	Ava_FORCEINLINE void RemovePrefix(iword count)
	{
		Ava_Assert(Base::m_f + count <= Base::m_l);
		Base::m_f = Base::m_f + count;
	}

	Ava_FORCEINLINE void RemoveSuffix(iword count)
	{
		Ava_Assert(Base::m_l - count >= Base::m_f);
		Base::m_l = Base::m_l - count;
	}


	Ava_FORCEINLINE bool CheckIndex(iword index) const
	{
		return Base::m_f + index < Base::m_l;
	}

	Ava_FORCEINLINE bool CheckRange(iword index, iword count) const
	{
		return (index | count) >= 0 &&
			Base::m_f + index + count <= Base::m_l;
	}
};

template<>
class Span<void> : public Private::Containers_Span::VoidLayer<void>
{
	typedef Private::Containers_Span::VoidLayer<void> Base;

public:
	using Base::Base;

#ifdef __INTELLISENSE__
	Span();
#endif
};

template<>
class Span<const void> : public Private::Containers_Span::VoidLayer<const void>
{
	typedef Private::Containers_Span::VoidLayer<const void> Base;

public:
	using Base::Base;

#ifdef __INTELLISENSE__
	Span();
#endif

	Ava_FORCEINLINE Span(void* data, uword size)
		: Base(static_cast<const void*>(data), size)
	{
	}
};

template<typename T>
Ava_FORCEINLINE iword Ava_Ext_Size(const Span<T>& span)
{
	return span.Size();
}

template<typename T>
Ava_FORCEINLINE bool operator==(const Span<T>& lhs, const Span<T>& rhs)
{
	return lhs.Begin() == rhs.Begin() && lhs.End() == rhs.End();
}

template<typename T>
Ava_FORCEINLINE bool operator!=(const Span<T>& lhs, const Span<T>& rhs)
{
	return lhs.Begin() != rhs.Begin() || lhs.End() != rhs.End();
}

#define Ava_SFINAE(...) \
	EnableIf<IsConvertibleTo<T1**, T2* const*> || \
		IsConvertibleTo<T2**, T1* const*>, __VA_ARGS__>

template<typename T1, typename T2>
Ava_FORCEINLINE Ava_SFINAE(bool) operator==(
	const Span<T1>& lhs, const Span<T2>& rhs)
{
	return lhs.Begin() == rhs.Begin() && lhs.End() == rhs.End();
}

template<typename T1, typename T2>
Ava_FORCEINLINE Ava_SFINAE(bool) operator!=(
	const Span<T1>& lhs, const Span<T2>& rhs)
{
	return lhs.Begin() != rhs.Begin() || lhs.End() != rhs.End();
}

#undef Ava_SFINAE

template<typename T> constexpr bool IsTriviallyEquatable<Span<T>> = true;
template<typename T> constexpr bool IsZeroConstructible<Span<T>> = true;

} // namespace Ava
