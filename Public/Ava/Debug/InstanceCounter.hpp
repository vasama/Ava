#pragma once

#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"

namespace Ava::Private::Debug_InstanceCounter {

template<typename T>
class Global
{
public:
	static Ava_FORCEINLINE void Increment()
	{
		++s_count;
	}

	static Ava_FORCEINLINE void Decrement()
	{
		--s_count;
	}

	static Ava_FORCEINLINE iword Count()
	{
		return s_count;
	}

private:
	static iword s_count;
};

template<typename T>
class Thread
{
public:
	static Ava_FORCEINLINE void Increment()
	{
		++s_count;
	}

	static Ava_FORCEINLINE void Decrement()
	{
		--s_count;
	}

	static Ava_FORCEINLINE iword Count()
	{
		return s_count;
	}

private:
	static thread_local iword s_count;
};

template<typename TCounter>
class Counter
{
protected:
	Ava_FORCEINLINE Counter()
	{
		TCounter::Increment();
	}

	Ava_FORCEINLINE Counter(Counter&&)
	{
		TCounter::Increment();
	}

	Ava_FORCEINLINE Counter(const Counter&)
	{
		TCounter::Increment();
	}

	Ava_FORCEINLINE Counter& operator=(Counter&&)
	{
		return *this;
	}

	Ava_FORCEINLINE Counter& operator=(const Counter&)
	{
		return *this;
	}

	Ava_FORCEINLINE ~Counter()
	{
		TCounter::Decrement();
	}
};

} // namespace Ava::Private::Debug_InstanceCounter

namespace Ava::Debug {

template<typename T>
using InstanceCounter = Private::Debug_InstanceCounter
	::Counter<Private::Debug_InstanceCounter::Global<T>>;

template<typename T>
using ThreadInstanceCounter = Private::Debug_InstanceCounter
	::Counter<Private::Debug_InstanceCounter::Thread<T>>;

template<typename T>
Ava_FORCEINLINE iword InstanceCount()
{
	return Private::Debug_InstanceCounter::Global<T>::Count();
}

template<typename T>
Ava_FORCEINLINE iword ThreadInstanceCount()
{
	return Private::Debug_InstanceCounter::Thread<T>::Count();
}

} // namespace Ava::Debug

template<typename T>
inline Ava::iword Ava::Private::Debug_InstanceCounter::Global<T>::s_count = 0;

template<typename T>
inline thread_local Ava::iword Ava::Private::Debug_InstanceCounter::Thread<T>::s_count = 0;
