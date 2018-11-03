#pragma once

#include "Ava/Debug/Assert.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"

namespace Ava {

namespace Private::Memory_RefCount {

struct Serial
{
	static Ava_FORCEINLINE iword IncrementRefCount(iword* refCount)
	{
		return ++*refCount;
	}

	static Ava_FORCEINLINE iword DecrementRefCount(iword* refCount)
	{
		return --*refCount;
	}
};

struct Atomic
{
	//TODO: implement atomic ref counting
};

template<typename TTraits>
struct BasicRefCount
{
public:
	Ava_FORCEINLINE iword IncrementRefCount()
	{
		return TTraits::IncrementRefCount(&m_refCount);
	}

	Ava_FORCEINLINE iword DecrementRefCount()
	{
		return TTraits::DecrementRefCount(&m_refCount);
	}

protected:
	Ava_FORCEINLINE BasicRefCount()
		: m_refCount(0)
	{
	}

	Ava_FORCEINLINE BasicRefCount(const BasicRefCount&)
		: m_refCount(0)
	{
	}

	Ava_FORCEINLINE BasicRefCount& operator=(const BasicRefCount&)
	{
		return *this;
	}

#if Ava_ASSERT_ACTIVE
	Ava_FORCEINLINE ~BasicRefCount()
	{
		Ava_Assert(m_refCount == 0);
	}
#endif

private:
	iword m_refCount;
};

} // namespace Private::Memory_RefCount

typedef Private::Memory_RefCount::BasicRefCount<
	Private::Memory_RefCount::Serial> RefCount;

typedef Private::Memory_RefCount::BasicRefCount<
	Private::Memory_RefCount::Atomic> AtomicRefCount;

struct RefCountTraits
{
	template<typename T>
	static Ava_FORCEINLINE iword IncrementRefCount(T* ptr)
	{
		return ptr->IncrementRefCount();
	}

	template<typename T>
	static Ava_FORCEINLINE iword DecrementRefCount(T* ptr)
	{
		return ptr->DecrementRefCount();
	}
};

} // namespace Ava
