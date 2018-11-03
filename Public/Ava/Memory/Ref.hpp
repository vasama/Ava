#pragma once

#include "Ava/Memory/DefaultDeleter.hpp"
#include "Ava/Memory/RefCount.hpp"
#include "Ava/Meta/EnableIf.hpp"
#include "Ava/Meta/Identity.hpp"
#include "Ava/Meta/Traits.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Private/Ebo.hpp"
#include "Ava/Private/Memory/SmartPtr.hpp"
#include "Ava/Utility/Move.hpp"

namespace Ava {

template<typename T, typename TDelete = DefaultDeleter, typename TTraits = RefCountTraits>
class Ref : Ava_EBO(Private::Memory_SmartPtr::Base<T>, TDelete)
{
	typedef Private::Memory_SmartPtr::Base<T> Base;

public:
	Ava_FORCEINLINE Ref()
		: TDelete()
	{
		Base::m_ptr = nullptr;
	}

	Ava_FORCEINLINE Ref(decltype(nullptr))
		: TDelete()
	{
		Base::m_ptr = nullptr;
	}

	Ava_FORCEINLINE Ref(T* ptr)
		: TDelete()
	{
		if (ptr) TTraits::IncrementRefCount(ptr);
		Base::m_ptr = ptr;
	}

	explicit Ava_FORCEINLINE Ref(TDelete deleter)
		: TDelete(Move(deleter))
	{
		Base::m_ptr = nullptr;
	}

	Ava_FORCEINLINE Ref(TDelete deleter, T* ptr)
		: TDelete(Move(deleter))
	{
		if (ptr) TTraits::IncrementRefCount(ptr);
		Base::m_ptr = ptr;
	}

#define Ava_SFINAE EnableIf<HasConv<TIn*, T*>>

	template<typename TIn, typename = Ava_SFINAE>
	Ava_FORCEINLINE Ref(Ref<TIn, TDelete, TTraits>&& other)
		: TDelete(static_cast<TDelete&&>(other))
	{
		Base::m_ptr = other.m_ptr;
		other.m_ptr = nullptr;
	}

	template<typename TIn, typename = Ava_SFINAE>
	Ava_FORCEINLINE Ref(const Ref<TIn, TDelete, TTraits>& other)
		: TDelete(static_cast<const TDelete&>(other))
	{
		T* ptr = other.m_ptr;
		if (ptr) TTraits::IncrementRefCount(ptr);
		Base::m_ptr = ptr;
	}

#undef Ava_SFINAE

	Ava_FORCEINLINE Ref(Ref&& other)
		: TDelete(static_cast<TDelete&&>(other))
	{
		Base::m_ptr = other.m_ptr;
		other.m_ptr = nullptr;
	}

	Ava_FORCEINLINE Ref(const Ref& other)
		: TDelete(static_cast<const TDelete&>(other))
	{
		T* ptr = other.m_ptr;
		if (ptr) TTraits::IncrementRefCount(ptr);
		Base::m_ptr = ptr;
	}

	Ref& operator=(decltype(nullptr))
	{
		if (T* old = Base::m_ptr)
			if (TTraits::DecrementRefCount(old) == 0)
				TDelete::operator()(old);

		Base::m_ptr = nullptr;

		return *this;
	}

	Ref& operator=(T* ptr)
	{
		if (T* old = Base::m_ptr)
			if (TTraits::DecrementRefCount(old) == 0)
				TDelete::operator()(old);

		if (ptr) TTraits::IncrementRefCount(ptr);
		Base::m_ptr = ptr;

		return *this;
	}

#define Ava_SFINAE(...) EnableIf<HasConv<TIn*, T*>, __VA_ARGS__>

	template<typename TIn>
	Ava_SFINAE(Ref&) operator=(Ref<TIn, TDelete, TTraits>&& other)
	{
		if (T* old = Base::m_ptr)
			if (TTraits::DecrementRefCount(old) == 0)
				TDelete::operator()(old);

		static_cast<TDelete&>(*this) =
			static_cast<TDelete&&>(other);

		Base::m_ptr = other.m_ptr;
		other.m_ptr = nullptr;

		return *this;
	}

	template<typename TIn>
	Ava_SFINAE(Ref&) operator=(const Ref<TIn, TDelete, TTraits>& other)
	{
		if (T* old = Base::m_ptr)
			if (TTraits::DecrementRefCount(old) == 0)
				TDelete::operator()(old);

		static_cast<TDelete&>(*this) =
			static_cast<const TDelete&>(other);

		T* ptr = other.m_ptr;
		if (ptr) TTraits::IncrementRefCount(ptr);
		Base::m_ptr = ptr;

		return *this;
	}

#undef Ava_SFINAE

	Ref& operator=(Ref&& other)
	{
		if (T* old = Base::m_ptr)
			if (TTraits::DecrementRefCount(old) == 0)
				TDelete::operator()(old);

		static_cast<TDelete&>(*this) =
			static_cast<TDelete&&>(other);

		Base::m_ptr = other.m_ptr;
		other.m_ptr = nullptr;

		return *this;
	}

	Ref& operator=(const Ref& other)
	{
		if (T* old = Base::m_ptr)
			if (TTraits::DecrementRefCount(old) == 0)
				TDelete::operator()(old);

		static_cast<TDelete&>(*this) =
			static_cast<const TDelete&>(other);

		T* ptr = other.m_ptr;
		if (ptr) TTraits::IncrementRefCount(ptr);
		Base::m_ptr = ptr;

		return *this;
	}

	Ref& Assign(TDelete deleter, T* ptr)
	{
		if (T* old = Base::m_ptr)
			if (TTraits::DecrementRefCount(old) == 0)
				TDelete::operator()(old);

		static_cast<TDelete&>(*this) = Move(deleter);

		if (ptr) TTraits::IncrementRefCount(ptr);
		Base::m_ptr = ptr;

		return *this;
	}

	Ava_FORCEINLINE ~Ref()
	{
		T* ptr = Base::m_ptr;
		if (ptr && TTraits::DecrementRefCount(ptr) == 0)
			TDelete::operator()(ptr);
	}

	Ava_FORCEINLINE T* Get() const
	{
		return Base::m_ptr;
	}

	Ava_FORCEINLINE T* Release()
	{
		T* ptr = Base::m_ptr;
		if (ptr)
		{
			Base::m_ptr = nullptr;
			TTraits::DecrementRefCount(ptr);
		}
		return ptr;
	}

	Ava_FORCEINLINE void Reset()
	{
		if (T* ptr = Base::m_ptr)
		{
			if (TTraits::DecrementRefCount(ptr))
				TDelete::operator()(ptr);
			Base::m_ptr = nullptr;
		}
	}

	Ava_FORCEINLINE const TDelete& GetDeleter() const
	{
		return static_cast<const TDelete&>(*this);
	}

	Ava_FORCEINLINE T& operator*() const
	{
		return *Base::m_ptr;
	}

	Ava_FORCEINLINE T* operator->() const
	{
		return Base::m_ptr;
	}

	explicit Ava_FORCEINLINE operator bool() const
	{
		return Base::m_ptr != nullptr;
	}

	Ava_FORCEINLINE bool operator!() const
	{
		return Base::m_ptr == nullptr;
	}

	template<typename, typename, typename>
	friend class Ref;
};

template<typename T, typename TDelete, typename TTraits>
Ava_FORCEINLINE bool operator==(const Ref<T, TDelete, TTraits>& lhs, const Ref<T, TDelete, TTraits>& rhs)
{
	return lhs.Get() == rhs.Get();
}

template<typename T, typename TDelete, typename TTraits>
Ava_FORCEINLINE bool operator!=(const Ref<T, TDelete, TTraits>& lhs, const Ref<T, TDelete, TTraits>& rhs)
{
	return lhs.Get() != rhs.Get();
}

template<typename T, typename TDelete, typename TTraits>
Ava_FORCEINLINE bool operator==(const Ref<T, TDelete, TTraits>& lhs, decltype(nullptr))
{
	return lhs.Get() == nullptr;
}

template<typename T, typename TDelete, typename TTraits>
Ava_FORCEINLINE bool operator==(decltype(nullptr), const Ref<T, TDelete, TTraits>& rhs)
{
	return rhs.Get() == nullptr;
}

template<typename T, typename TDelete, typename TTraits>
Ava_FORCEINLINE bool operator!=(const Ref<T, TDelete, TTraits>& lhs, decltype(nullptr))
{
	return lhs.Get() != nullptr;
}

template<typename T, typename TDelete, typename TTraits>
Ava_FORCEINLINE bool operator!=(decltype(nullptr), const Ref<T, TDelete, TTraits>& rhs)
{
	return rhs.Get() != nullptr;
}

template<typename T, typename TDelete, typename TTraits>
Ava_FORCEINLINE bool operator==(const Ref<T, TDelete, TTraits>& lhs, const Identity<T>* rhs)
{
	return lhs.Get() == rhs;
}

template<typename T, typename TDelete, typename TTraits>
Ava_FORCEINLINE bool operator==(const Identity<T>* lhs, const Ref<T, TDelete, TTraits>& rhs)
{
	return lhs == rhs.Get();
}

template<typename T, typename TDelete, typename TTraits>
Ava_FORCEINLINE bool operator!=(const Ref<T, TDelete, TTraits>& lhs, const Identity<T>* rhs)
{
	return lhs.Get() != rhs;
}

template<typename T, typename TDelete, typename TTraits>
Ava_FORCEINLINE bool operator!=(const Identity<T>* lhs, const Ref<T, TDelete, TTraits>& rhs)
{
	return lhs != rhs.Get();
}

template<typename T, typename TDelete, typename TTraits>
constexpr bool IsTriviallyRelocatable<Ref<T, TDelete, TTraits>> = IsTriviallyRelocatable<TDelete>;

template<typename T, typename TDelete, typename TTraits>
constexpr bool IsTriviallyEquatable<Ref<T, TDelete, TTraits>> = IsEmpty<TDelete>;

template<typename T, typename TDelete, typename TTraits>
constexpr bool IsZeroConstructible<Ref<T, TDelete, TTraits>> = IsZeroConstructible<TDelete>;

} // namespace Ava
