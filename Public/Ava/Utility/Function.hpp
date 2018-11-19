#pragma once

#include "Ava/Debug/Assert.hpp"
#include "Ava/Memory/AlignedStorage.hpp"
#include "Ava/Memory/DefaultAllocator.hpp"
#include "Ava/Memory/Utility.hpp"
#include "Ava/Meta/EnableIf.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Private/Ebo.hpp"
#include "Ava/Utility/Forward.hpp"
#include "Ava/Utility/Move.hpp"

namespace Ava {

namespace Private::Utility_Function {

template<typename TResult, typename... TParams>
struct Abstract
{
	virtual TResult Invoke(TParams... args) const = 0;
	virtual void Destroy_NoDeallocate() = 0;
	virtual void Destroy_Deallocate() = 0;
	virtual void Relocate(Abstract* area) = 0;
	virtual void Copy(Abstract* area) const = 0;
	virtual Abstract* Clone() const = 0;

	Ava_FORCEINLINE void Destroy(bool deallocate)
	{
		if (deallocate)
			Destroy_Deallocate();
		else
			Destroy_NoDeallocate();
	}
};

template<typename TFn, typename TResult, typename... TParams>
struct CallableLayer : Ava_EBO(Abstract<TResult, TParams...>, TFn)
{
	Ava_FORCEINLINE CallableLayer(TFn fn)
		: TFn(fn)
	{
	}
};

template<typename TFn, typename TAllocator, typename TResult, typename... TParams>
struct Concrete : Ava_EBO(CallableLayer<TFn, TResult, TParams...>, TAllocator)
{
	typedef Abstract<TResult, TParams...> AbstractType;

	Ava_FORCEINLINE Concrete(TFn fn, TAllocator alloc)
		: CallableLayer<TFn, TResult, TParams...>(fn), TAllocator(alloc)
	{
	}

	TResult Invoke(TParams... args) const override
	{
		return TFn::operator()(args...);
	}

	virtual void Relocate(AbstractType* area) override
	{
		Ava::Relocate<Concrete>((Concrete*)area, this);
	}

	virtual void Destroy_NoDeallocate() override
	{
		Ava::Destroy<Concrete>(this);
	}

	virtual void Destroy_Deallocate() override
	{
		Delete(TAllocator(*this), this);
	}

	virtual void Copy(AbstractType* area) const override
	{
		CopyConstruct<Concrete>((Concrete*)area, *this);
	}

	virtual AbstractType* Clone() const override
	{
		Concrete* area = (Concrete*)TAllocator::Allocate(sizeof(Concrete));
		CopyConstruct<Concrete>(area, *this);
		return area;
	}
};

template<typename TResult, typename... TParams>
struct Base
{
	Abstract<TResult, TParams...>* m_abstract;
};

template<uword TSize>
class StorageLayer
{
public:
	Ava_FORCEINLINE void* GetStorage() const
	{
		return &m_storage;
	}

	Ava_FORCEINLINE bool IsRemote(const void* storage) const
	{
		return storage == &m_storage;
	}

	static constexpr Ava_FORCEINLINE bool Fits(uword size)
	{
		return size <= sizeof(m_storage);
	}

private:
	AlignedStorage<
		TSize + sizeof(void*), alignof(void*)
	> mutable m_storage;
};

template<>
class StorageLayer<0>
{
public:
	Ava_FORCEINLINE void* GetStorage() const
	{
		return nullptr;
	}

	constexpr Ava_FORCEINLINE bool IsRemote(const void*) const
	{
		return true;
	}

	static constexpr Ava_FORCEINLINE bool Fits(uword)
	{
		return false;
	}
};

} // namespace Private::Utility_Function

template<typename TSignature, uword TLocalSize, bool TMayAllocate>
class BasicFunction;

Ava_MSVC_WARNING(push)
Ava_MSVC_WARNING(disable: 4127) // constant expression in conditional
template<typename TResult, typename... TParams, uword TLocalSize, bool TMayAllocate>
class BasicFunction<TResult(TParams...), TLocalSize, TMayAllocate>
	: Ava_EBO(Private::Utility_Function::Base<TResult, TParams...>,
		Private::Utility_Function::StorageLayer<TLocalSize>)
{
	typedef Private::Utility_Function::Base<TResult, TParams...> Base;
	typedef Private::Utility_Function::StorageLayer<TLocalSize> Storage;

	typedef Private::Utility_Function::Abstract<TResult, TParams...> AbstractType;

public:
	Ava_FORCEINLINE BasicFunction()
	{
		Base::m_abstract = nullptr;
	}

#ifdef __INTELLISENSE__
#	define Ava_SFINAE(...) __VA_ARGS__
#else
#	define Ava_SFINAE(...) EnableIf<!IsSame<Decay<TFn>, BasicFunction>, __VA_ARGS__>
#endif

	template<typename TFn, Ava_SFINAE(int) = 0>
	Ava_FORCEINLINE BasicFunction(TFn&& fn)
	{
		Initialize(Forward<TFn>(fn), DefaultAllocator());
	}

	template<typename TFn, typename TAllocator, Ava_SFINAE(int) = 0>
	Ava_FORCEINLINE BasicFunction(TFn&& fn, const TAllocator& alloc)
	{
		Initialize(Forward<TFn>(fn), alloc);
	}

	Ava_FORCEINLINE BasicFunction(decltype(nullptr))
	{
		Base::m_abstract = nullptr;
	}

	BasicFunction(BasicFunction&& other)
	{
		if (AbstractType* src = other.m_abstract)
		{
			if (TMayAllocate && other.Storage::IsRemote(src))
			{
				Base::m_abstract = src;
			}
			else
			{
				AbstractType* abstract = (AbstractType*)Storage::GetStorage();
				src->Relocate(abstract);
				Base::m_abstract = abstract;
			}
			other.m_abstract = nullptr;
		}
		else Base::m_abstract = nullptr;
	}

	BasicFunction(const BasicFunction& other)
	{
		if (const AbstractType* src = other.m_abstract)
		{
			if (TMayAllocate && Storage::IsRemote(src))
			{
				Base::m_abstract = src->Clone();
			}
			else
			{
				AbstractType* abstract = (AbstractType*)Storage::GetStorage();
				src->Copy(abstract);
				Base::m_abstract = abstract;
			}
		}
		else Base::m_abstract = nullptr;
	}

	template<typename TFn>
	Ava_SFINAE(BasicFunction&) operator=(TFn&& fn)
	{
		Clean();
		Initialize(Forward<TFn>(fn), DefaultAllocator());
		return *this;
	}

	Ava_FORCEINLINE BasicFunction& operator=(decltype(nullptr))
	{
		if (AbstractType* abstract = Base::m_abstract)
		{
			abstract->Destroy(TMayAllocate && Storage::IsRemote(abstract));
			Base::m_abstract = nullptr;
		}
		return *this;
	}

	BasicFunction& operator=(BasicFunction&& other)
	{
		Clean();

		if (AbstractType* src = other.m_abstract)
		{
			if (TMayAllocate && other.Storage::IsRemote(src))
			{
				Base::m_abstract = src;
			}
			else
			{
				AbstractType* abstract = (AbstractType*)Storage::GetStorage();
				src->Relocate(abstract);
				Base::m_abstract = abstract;
			}
			other.m_abstract = nullptr;
		}
		else Base::m_abstract = nullptr;

		return *this;
	}

	BasicFunction& operator=(const BasicFunction& other)
	{
		Clean();

		if (const AbstractType* src = other.m_abstract)
		{
			if (TMayAllocate && Storage::IsRemote(src))
			{
				Base::m_abstract = src->Clone();
			}
			else
			{
				AbstractType* abstract = (AbstractType*)Storage::GetStorage();
				src->Copy(abstract);
				Base::m_abstract = abstract;
			}
		}
		else Base::m_abstract = nullptr;

		return *this;
	}

	template<typename TFn, typename TAllocator>
	Ava_SFINAE(BasicFunction&) Assign(TFn&& fn, const TAllocator& alloc)
	{
		Clean();
		Initialize(Forward<TFn>(fn), alloc);
		return *this;
	}

	Ava_FORCEINLINE ~BasicFunction()
	{
		Clean();
	}

#undef Ava_SFINAE


	Ava_FORCEINLINE TResult operator()(TParams... args) const
	{
		Ava_AssertSlow(Base::m_abstract);
		return Base::m_abstract->Invoke(args...);
	}


	explicit Ava_FORCEINLINE operator bool() const
	{
		return Base::m_abstract != nullptr;
	}

	Ava_FORCEINLINE bool operator!() const
	{
		return Base::m_abstract == nullptr;
	}

private:
	Ava_FORCEINLINE void Clean()
	{
		if (AbstractType* abstract = Base::m_abstract)
			abstract->Destroy(TMayAllocate && Storage::IsRemote(abstract));
	}

	template<typename TFn, typename TAllocator>
	void Initialize(TFn fn, TAllocator alloc)
	{
		typedef Private::Utility_Function::Concrete<
			TFn, DefaultAllocator, TResult, TParams...> Local;

		if constexpr (Storage::Fits(sizeof(Local)))
		{
			Ava_UNUSED(alloc);

			typedef Local Concrete;

			Concrete* storage = (Concrete*)Storage::GetStorage();
			Construct<Concrete>(storage, Move(fn), DefaultAllocator());
			Base::m_abstract = storage;
		}
		else
		{
			static_assert(TMayAllocate);

			typedef Private::Utility_Function::Concrete<
				TFn, TAllocator, TResult, TParams...> Concrete;

			Concrete* storage = (Concrete*)alloc.Allocate(sizeof(Concrete));
			Construct<Concrete>(storage, Move(fn), alloc);
			Base::m_abstract = storage;
		}
	}
};
Ava_MSVC_WARNING(pop)

template<typename TSignature, uword TSize, bool TAlloc>
Ava_FORCEINLINE bool operator==(const BasicFunction<TSignature, TSize, TAlloc>& lhs, decltype(nullptr))
{
	return !lhs;
}

template<typename TSignature, uword TSize, bool TAlloc>
Ava_FORCEINLINE bool operator==(decltype(nullptr), const BasicFunction<TSignature, TSize, TAlloc>& rhs)
{
	return !rhs;
}

template<typename TSignature, uword TSize, bool TAlloc>
Ava_FORCEINLINE bool operator!=(const BasicFunction<TSignature, TSize, TAlloc>& lhs, decltype(nullptr))
{
	return (bool)lhs;
}

template<typename TSignature, uword TSize, bool TAlloc>
Ava_FORCEINLINE bool operator!=(decltype(nullptr), const BasicFunction<TSignature, TSize, TAlloc>& rhs)
{
	return (bool)rhs;
}


template<typename TSignature>
using Function = BasicFunction<TSignature, 0, 1>;

template<typename TSignature, uword TLocalSize = sizeof(void*) * 2>
using SmallFunction = BasicFunction<TSignature, TLocalSize, 1>;

template<typename TSignature, uword TLocalSize = sizeof(void*) * 2>
using InplaceFunction = BasicFunction<TSignature, TLocalSize, 0>;

} // namespace Ava
