#pragma once

#include "Ava/Memory/DefaultDeleter.hpp"
#include "Ava/Meta/EnableIf.hpp"
#include "Ava/Meta/Identity.hpp"
#include "Ava/Meta/Traits.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Private/Ebo.hpp"
#include "Ava/Private/Memory/SmartPtr.hpp"
#include "Ava/Utility/Move.hpp"

namespace Ava {

template<typename T, typename TDelete = DefaultDeleter>
class Owner : Ava_EBO(Private::Memory_SmartPtr::Base<T>, TDelete)
{
	typedef Private::Memory_SmartPtr::Base<T> Base;

public:
	Ava_FORCEINLINE Owner()
		: TDelete()
	{
		Base::m_ptr = nullptr;
	}

	Ava_FORCEINLINE Owner(decltype(nullptr))
		: TDelete()
	{
		Base::m_ptr = nullptr;
	}

	Ava_FORCEINLINE Owner(T* ptr)
		: TDelete()
	{
		Base::m_ptr = ptr;
	}

	template<typename TIn>
	Owner(TIn* ptr) = delete;

	explicit Ava_FORCEINLINE Owner(TDelete deleter)
		: TDelete(Move(deleter))
	{
		Base::m_ptr = nullptr;
	}

	Ava_FORCEINLINE Owner(TDelete deleter, T* ptr)
		: TDelete(Move(deleter))
	{
		Base::m_ptr = ptr;
	}

	template<typename TIn>
	Owner(TDelete deleter, TIn* ptr) = delete;

	Ava_FORCEINLINE Owner(Owner&& other)
	{
		Base::m_ptr = other.m_ptr;
		other.m_ptr = nullptr;
	}

	Owner(const Owner&) = delete;

	Ava_FORCEINLINE Owner& operator=(decltype(nullptr))
	{
		if (T* old = Base::m_ptr)
			TDelete::operator()(old);

		Base::m_ptr = nullptr;

		return *this;
	}

	Ava_FORCEINLINE Owner& operator=(T* ptr)
	{
		if (T* old = Base::m_ptr)
			TDelete::operator()(old);

		Base::m_ptr = ptr;

		return *this;
	}

	template<typename TIn>
	Owner& operator=(TIn* ptr) = delete;

	Owner& operator=(Owner&& other)
	{
		if (T* old = Base::m_ptr)
			TDelete::operator()(old);

		static_cast<TDelete&>(*this) =
			static_cast<TDelete&&>(other);

		Base::m_ptr = other.m_ptr;
		other.m_ptr = nullptr;

		return *this;
	}

	Owner& operator=(const Owner&) = delete;

	Ava_FORCEINLINE Owner& Assign(TDelete deleter, T* ptr)
	{
		if (T* old = Base::m_ptr)
			TDelete::operator()(old);

		static_cast<TDelete&>(*this) = Move(deleter);

		Base::m_ptr = ptr;

		return *this;
	}

	template<typename TIn>
	Owner& Assign(TDelete deleter, TIn* ptr) = delete;

	Ava_FORCEINLINE ~Owner()
	{
		if (T* ptr = Base::m_ptr)
			TDelete::operator()(ptr);
	}

	Ava_FORCEINLINE T* Get() const
	{
		return Base::m_ptr;
	}

	Ava_FORCEINLINE T* Release()
	{
		T* ptr = Base::m_ptr;
		Base::m_ptr = nullptr;
		return ptr;
	}

	Ava_FORCEINLINE void Reset()
	{
		if (T* ptr = Base::m_ptr)
		{
			Base::m_ptr = nullptr;
			TDelete::operator()(ptr);
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

	template<typename, typename>
	friend class Owner;
};

template<typename T, typename TDelete>
Ava_FORCEINLINE bool operator==(const Owner<T, TDelete>& lhs, const Owner<T, TDelete>& rhs)
{
	return lhs.Get() == rhs.Get();
}

template<typename T, typename TDelete>
Ava_FORCEINLINE bool operator!=(const Owner<T, TDelete>& lhs, const Owner<T, TDelete>& rhs)
{
	return lhs.Get() != rhs.Get();
}

template<typename T, typename TDelete>
Ava_FORCEINLINE bool operator==(const Owner<T, TDelete>& lhs, decltype(nullptr))
{
	return lhs.Get() == nullptr;
}

template<typename T, typename TDelete>
Ava_FORCEINLINE bool operator==(decltype(nullptr), const Owner<T, TDelete>& rhs)
{
	return rhs.Get() == nullptr;
}

template<typename T, typename TDelete>
Ava_FORCEINLINE bool operator!=(const Owner<T, TDelete>& lhs, decltype(nullptr))
{
	return lhs.Get() != nullptr;
}

template<typename T, typename TDelete>
Ava_FORCEINLINE bool operator!=(decltype(nullptr), const Owner<T, TDelete>& rhs)
{
	return rhs.Get() != nullptr;
}

template<typename T, typename TDelete>
Ava_FORCEINLINE bool operator==(const Owner<T, TDelete>& lhs, const Identity<T>* rhs)
{
	return lhs.Get() == rhs;
}

template<typename T, typename TDelete>
Ava_FORCEINLINE bool operator==(const Identity<T>* lhs, const Owner<T, TDelete>& rhs)
{
	return lhs == rhs.Get();
}

template<typename T, typename TDelete>
Ava_FORCEINLINE bool operator!=(const Owner<T, TDelete>& lhs, const Identity<T>* rhs)
{
	return lhs.Get() != rhs;
}

template<typename T, typename TDelete>
Ava_FORCEINLINE bool operator!=(const Identity<T>* lhs, const Owner<T, TDelete>& rhs)
{
	return lhs != rhs.Get();
}

template<typename T, typename TDelete>
constexpr bool IsTriviallyRelocatable<Owner<T, TDelete>> = IsTriviallyRelocatable<TDelete>;

template<typename T, typename TDelete>
constexpr bool IsTriviallyEquatable<Owner<T, TDelete>> = IsEmpty<TDelete>;

template<typename T, typename TDelete>
constexpr bool IsZeroConstructible<Owner<T, TDelete>> = IsZeroConstructible<TDelete>;

} // namespace Ava
