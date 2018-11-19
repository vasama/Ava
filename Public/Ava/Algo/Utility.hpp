#pragma once

#include "Ava/Memory/Memory.hpp"
#include "Ava/Memory/Utility.hpp"
#include "Ava/Meta/Iterator.hpp"
#include "Ava/Meta/Traits.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Private/PointerMath.hpp"
#include "Ava/Types.hpp"
#include "Ava/Utility/ConstParam.hpp"
#include "Ava/Utility/Move.hpp"

namespace Ava::Private::Algo_Utility {

template<typename T, typename TIterator>
void ValueConstruct_Iter(TIterator first, TIterator last)
{
	for (; first != last; ++first)
		ValueConstruct<T>(&*first);
}

template<typename T, typename TIterator>
void ValueConstruct_Size(TIterator first, uword size)
{
	for (; size > 0; --size, (void) ++first)
		ValueConstruct<T>(&*first);
}

template<typename T, typename TIterator, typename... TArgs>
void Construct_Iter(TIterator first, TIterator last, ConstParam<TArgs>... args)
{
	for (; first != last; ++first)
		Construct<T>(&*first, args...);
}

template<typename T, typename TIterator, typename... TArgs>
void Construct_Size(TIterator first, uword count, ConstParam<TArgs>... args)
{
	for (; count > 0; --count, (void) ++first)
		Construct<T>(&*first, args...);
}

template<typename T, typename TIterator, typename TSrcIterator>
void MoveConstruct_Iter(TIterator first, TSrcIterator srcFirst, TSrcIterator srcLast)
{
	for (; srcFirst != srcLast; (void) ++first, (void) ++srcFirst)
		MoveConstruct<T>(&*first, Move(*srcFirst));
}

template<typename T, typename TIterator, typename TSrcIterator>
void MoveConstruct_Size(TIterator first, TSrcIterator srcFirst, uword count)
{
	for (; count > 0; --count, (void) ++first, (void) ++srcFirst)
		MoveConstruct<T>(&*first, Move(*srcFirst));
}

template<typename T, typename TIterator, typename TSrcIterator>
void CopyConstruct_Iter(TIterator first, TSrcIterator srcFirst, TSrcIterator srcLast)
{
	for (; srcFirst != srcLast; (void) ++first, (void) ++srcFirst)
		CopyConstruct<T>(&*first, *srcFirst);
}

template<typename T, typename TIterator, typename TSrcIterator>
void CopyConstruct_Size(TIterator first, TSrcIterator srcFirst, uword count)
{
	for (; count > 0; --count, (void) ++first, (void) ++srcFirst)
		MoveConstruct<T>(&*first, *srcFirst);
}

template<typename T, typename TIterator, typename TSrcIterator>
void RelocateFwd_Iter(TIterator first, TSrcIterator srcFirst, TSrcIterator srcLast)
{
	for (; srcFirst != srcLast; (void) ++first, (void) ++srcFirst)
		Relocate<T>(&*first, &*srcFirst);
}

template<typename T, typename TIterator, typename TSrcIterator>
void RelocateFwd_Size(TIterator first, TSrcIterator srcFirst, uword count)
{
	for (; count > 0; --count, (void) ++first, (void) ++srcFirst)
		Relocate<T>(&*first, &*srcFirst);
}

template<typename T, typename TIterator, typename TSrcIterator>
void RelocateBwd_Iter(TIterator last, TSrcIterator srcLast, TSrcIterator srcFirst)
{
	while (srcFirst != srcLast)
		Relocate<T>(&*--last, &*--srcLast);
}

template<typename T, typename TIterator, typename TSrcIterator>
void RelocateBwd_Size(TIterator last, TSrcIterator srcLast, uword count)
{
	for (; count > 0; --count)
		Relocate<T>(&*--last, &*--srcLast);
}

template<typename T, typename TIterator>
void Destroy_Iter(TIterator first, TIterator last)
{
	for (; first != last; ++first)
		Destroy<T>(&*first);
}

template<typename T, typename TIterator>
void Destroy_Size(TIterator first, uword size)
{
	for (; size > 0; --size, (void) ++first)
		Destroy<T>(&*first);
}

template<typename TIterator, typename TSrcIterator>
void Move_Iter(TIterator first, TSrcIterator srcFirst, TSrcIterator srcLast)
{
	for (; srcFirst != srcLast; (void) ++first, (void) ++srcFirst)
		*first = Move(*srcFirst);
}

template<typename TIterator, typename TSrcIterator>
void Move_Size(TIterator first, TSrcIterator srcFirst, uword count)
{
	for (; count > 0; --count, (void) ++first, (void) ++srcFirst)
		*first = Move(*srcFirst);
}

template<typename TIterator, typename TSrcIterator>
void Copy_Iter(TIterator first, TSrcIterator srcFirst, TSrcIterator srcLast)
{
	for (; srcFirst != srcLast; (void) ++first, (void) ++srcFirst)
		*first = *srcFirst;
}

template<typename TIterator, typename TSrcIterator>
void Copy_Size(TIterator first, TSrcIterator srcFirst, uword count)
{
	for (; count > 0; --count, (void) ++first, (void) ++srcFirst)
		*first = *srcFirst;
}

} // namespace Ava::Private::Algo_Utility

namespace Ava::Algo {

template<typename TIterator>
Ava_FORCEINLINE void ValueConstruct(TIterator first, TIterator last)
{
	typedef IteratorValue<TIterator> T;

	if constexpr (IsContiguousIterator<TIterator> && IsZeroConstructible<T>)
	{
		Memory::Zero(&*first, Ava_PTRSUB(&*last, &*first));
	}
	else
	{
		Private::Algo_Utility::ValueConstruct_Iter<T>(first, last);
	}
}

template<typename TIterator>
Ava_FORCEINLINE void ValueConstruct(TIterator first, uword count)
{
	typedef IteratorValue<TIterator> T;

	if constexpr (IsContiguousIterator<TIterator> && IsZeroConstructible<T>)
	{
		Memory::Zero(&*first, count * sizeof(T));
	}
	else
	{
		if constexpr (IteratorCategory::Get<TIterator> >= IteratorCategory::RandomAccess)
		{
			Private::Algo_Utility::ValueConstruct_Iter<T>(first, first + count);
		}
		else
		{
			Private::Algo_Utility::ValueConstruct_Size<T>(first, count);
		}
	}
}

template<typename TIterator>
Ava_FORCEINLINE void DefaultConstruct(TIterator first, TIterator last)
{
	typedef IteratorValue<TIterator> T;

	if constexpr (IsTriviallyConstructible<T>)
	{
		Ava_UNUSED(first, last);
	}
	else
	{
		ValueConstruct(first, last);
	}
}

template<typename TIterator>
Ava_FORCEINLINE void DefaultConstruct(TIterator first, uword count)
{
	typedef IteratorValue<TIterator> T;

	if constexpr (IsTriviallyConstructible<T>)
	{
		Ava_UNUSED(first, count);
	}
	else
	{
		ValueConstruct(first, count);
	}
}

template<typename TIterator>
Ava_FORCEINLINE void Construct(TIterator first, TIterator last)
{
	ValueConstruct(first, last);
}

template<typename TIterator>
Ava_FORCEINLINE void Construct(TIterator first, uword count)
{
	ValueConstruct(first, count);
}

template<typename TIterator, typename... TArgs>
Ava_FORCEINLINE void Construct(TIterator first, TIterator last, const TArgs&... args)
{
	Private::Algo_Utility::Construct_Iter<IteratorValue<TIterator>,
		TIterator, TArgs...>(first, last, args...);
}

template<typename TIterator, typename... TArgs>
Ava_FORCEINLINE void Construct(TIterator first, uword count, const TArgs&... args)
{
	if constexpr (IteratorCategory::Get<TIterator> >= IteratorCategory::RandomAccess)
	{
		Private::Algo_Utility::Construct_Iter<IteratorValue<TIterator>,
			TIterator, TArgs...>(first, first + count, args...);
	}
	else
	{
		Private::Algo_Utility::Construct_Size<IteratorValue<TIterator>,
			TIterator, TArgs...>(first, count, args...);
	}
}

template<typename TIterator, typename TSrcIterator>
Ava_FORCEINLINE void MoveConstruct(TIterator first, TSrcIterator srcFirst, TSrcIterator srcLast)
{
	typedef IteratorValue<TIterator> T;
	static_assert(IsSame<IteratorValue<TSrcIterator>, T>);

	if constexpr (IsContiguousIterator<TSrcIterator> &&
		IsContiguousIterator<TIterator> && IsTriviallyCopyable<T>)
	{
		Memory::Copy(&*first, &*srcFirst, Ava_PTRSUB(&*srcLast, &*srcFirst));
	}
	else
	{
		Private::Algo_Utility::MoveConstruct_Iter<T>(first, srcFirst, srcLast);
	}
}

template<typename TIterator, typename TSrcIterator>
Ava_FORCEINLINE void MoveConstruct(TIterator first, TSrcIterator srcFirst, uword count)
{
	typedef IteratorValue<TIterator> T;
	static_assert(IsSame<IteratorValue<TSrcIterator>, T>);

	if constexpr (IsContiguousIterator<TSrcIterator> &&
		IsContiguousIterator<TIterator> && IsTriviallyCopyable<T>)
	{
		Memory::Copy(&*first, &*srcFirst, count * sizeof(T));
	}
	else
	{
		if constexpr (IteratorCategory::Get<TSrcIterator> >= IteratorCategory::RandomAccess)
		{
			Private::Algo_Utility::MoveConstruct_Iter<T>(first, srcFirst, srcFirst + count);
		}
		else
		{
			Private::Algo_Utility::MoveConstruct_Size<T>(first, srcFirst, count);
		}
	}
}

template<typename TIterator, typename TSrcIterator>
Ava_FORCEINLINE void CopyConstruct(TIterator first, TSrcIterator srcFirst, TSrcIterator srcLast)
{
	typedef IteratorValue<TIterator> T;
	static_assert(IsSame<RemoveConst<IteratorValue<TSrcIterator>>, T>);

	if constexpr (IsContiguousIterator<TSrcIterator> &&
		IsContiguousIterator<TIterator> && IsTriviallyCopyable<T>)
	{
		Memory::Copy(&*first, &*srcFirst, Ava_PTRSUB(&*srcLast, &*srcFirst));
	}
	else
	{
		Private::Algo_Utility::CopyConstruct_Iter<T>(first, srcFirst, srcLast);
	}
}

template<typename TIterator, typename TSrcIterator>
Ava_FORCEINLINE void CopyConstruct(TIterator first, TSrcIterator srcFirst, uword count)
{
	typedef IteratorValue<TIterator> T;
	static_assert(IsSame<RemoveConst<IteratorValue<TSrcIterator>>, T>);

	if constexpr (IsContiguousIterator<TSrcIterator> &&
		IsContiguousIterator<TIterator> && IsTriviallyCopyable<T>)
	{
		Memory::Copy(&*first, &*srcFirst, count * sizeof(T));
	}
	else
	{
		if constexpr (IteratorCategory::Get<TSrcIterator> >= IteratorCategory::RandomAccess)
		{
			Private::Algo_Utility::CopyConstruct_Iter<T>(first, srcFirst, srcFirst + count);
		}
		else
		{
			Private::Algo_Utility::CopyConstruct_Size<T>(first, srcFirst, count);
		}
	}
}

template<typename TIterator, typename TSrcIterator>
Ava_FORCEINLINE void RelocateFwd(TIterator first, TSrcIterator srcFirst, TSrcIterator srcLast)
{
	typedef IteratorValue<TIterator> T;
	static_assert(IsSame<IteratorValue<TSrcIterator>, T>);

	if constexpr (IsContiguousIterator<TSrcIterator> &&
		IsContiguousIterator<TIterator> && IsTriviallyRelocatable<T>)
	{
		Memory::MoveFwd(&*first, &*srcFirst, Ava_PTRSUB(&*srcLast, &*srcFirst));
	}
	else
	{
		Private::Algo_Utility::RelocateFwd_Iter<T>(first, srcFirst, srcLast);
	}
}

template<typename TIterator, typename TSrcIterator>
Ava_FORCEINLINE void RelocateFwd(TIterator first, TSrcIterator srcFirst, uword count)
{
	typedef IteratorValue<TIterator> T;
	static_assert(IsSame<IteratorValue<TSrcIterator>, T>);

	if constexpr (IsContiguousIterator<TSrcIterator> &&
		IsContiguousIterator<TIterator> && IsTriviallyRelocatable<T>)
	{
		Memory::MoveFwd(&*first, &*srcFirst, count * sizeof(T));
	}
	else
	{
		if constexpr (IteratorCategory::Get<TSrcIterator> >= IteratorCategory::RandomAccess)
		{
			Private::Algo_Utility::RelocateFwd_Iter<T>(first, srcFirst, srcFirst + count);
		}
		else
		{
			Private::Algo_Utility::RelocateFwd_Size<T>(first, srcFirst, count);
		}
	}
}

template<typename TIterator, typename TSrcIterator>
Ava_FORCEINLINE void RelocateBwd(TIterator last, TSrcIterator srcLast, TSrcIterator srcFirst)
{
	typedef IteratorValue<TIterator> T;
	static_assert(IsSame<IteratorValue<TSrcIterator>, T>);

	if constexpr (IsContiguousIterator<TSrcIterator> &&
		IsContiguousIterator<TIterator> && IsTriviallyRelocatable<T>)
	{
		uword size = Ava_PTRSUB(&*srcLast, &*srcFirst);
		Memory::MoveBwd((byte*)&*last - size, &*srcFirst, size);
	}
	else
	{
		Private::Algo_Utility::RelocateBwd_Iter<T>(last, srcLast, srcFirst);
	}
}

template<typename TIterator, typename TSrcIterator>
Ava_FORCEINLINE void RelocateBwd(TIterator last, TSrcIterator srcLast, uword count)
{
	typedef IteratorValue<TIterator> T;
	static_assert(IsSame<IteratorValue<TSrcIterator>, T>);

	if constexpr (IsContiguousIterator<TSrcIterator> &&
		IsContiguousIterator<TIterator> && IsTriviallyRelocatable<T>)
	{
		uword size = count * sizeof(T);
		Memory::MoveBwd((byte*)&*last - size, (const byte*)&*srcLast - size, size);
	}
	else
	{
		if constexpr (IteratorCategory::Get<TSrcIterator> >= IteratorCategory::RandomAccess)
		{
			Private::Algo_Utility::RelocateBwd_Iter<T>(last, srcLast, srcLast - count);
		}
		else
		{
			Private::Algo_Utility::RelocateBwd_Size<T>(last, srcLast, count);
		}
	}
}

template<typename TIterator>
Ava_FORCEINLINE void Destroy(TIterator first, TIterator last)
{
	typedef IteratorValue<TIterator> T;

	if constexpr (IsTriviallyDestructible<T>)
	{
		Ava_UNUSED(first, last);
	}
	else
	{
		Private::Algo_Utility::Destroy_Iter<T>(first, last);
	}
}

template<typename TIterator>
Ava_FORCEINLINE void Destroy(TIterator first, uword count)
{
	typedef IteratorValue<TIterator> T;

	if constexpr (IsTriviallyDestructible<T>)
	{
		Ava_UNUSED(first, count);
	}
	else
	{
		if constexpr (IteratorCategory::Get<TIterator> >= IteratorCategory::RandomAccess)
		{
			Private::Algo_Utility::Destroy_Iter<T>(first, first + count);
		}
		else
		{
			Private::Algo_Utility::Destroy_Size<T>(first, count);
		}
	}
}

template<typename TIterator, typename TSrcIterator>
Ava_FORCEINLINE void MoveFwd(TIterator first, TSrcIterator srcFirst, TSrcIterator srcLast)
{
	typedef IteratorValue<TIterator> T;
	static_assert(IsSame<RemoveConst<IteratorValue<TIterator>>, T>);

	if constexpr (IsContiguousIterator<TSrcIterator> &&
		IsContiguousIterator<TIterator> && IsTriviallyCopyable<T>)
	{
		Memory::Copy(&*first, &*srcFirst, Ava_PTRSUB(&*srcLast, &*srcFirst));
	}
	else
	{
		Private::Algo_Utility::Move_Iter(first, srcFirst, srcLast);
	}
}

template<typename TIterator, typename TSrcIterator>
Ava_FORCEINLINE void MoveFwd(TIterator first, TSrcIterator srcFirst, uword count)
{
	typedef IteratorValue<TIterator> T;
	static_assert(IsSame<RemoveConst<IteratorValue<TIterator>>, T>);

	if constexpr (IsContiguousIterator<TSrcIterator> &&
		IsContiguousIterator<TIterator> && IsTriviallyCopyable<T>)
	{
		Memory::Copy(&*first, &*srcFirst, count * sizeof(T));
	}
	else
	{
		if constexpr (IteratorCategory::Get<TSrcIterator> >= IteratorCategory::RandomAccess)
		{
			Private::Algo_Utility::Move_Iter(first, srcFirst, srcFirst + count);
		}
		else
		{
			Private::Algo_Utility::Move_Size(first, srcFirst, count);
		}
	}
}

template<typename TIterator, typename TSrcIterator>
Ava_FORCEINLINE void CopyFwd(TIterator first, TSrcIterator srcFirst, TSrcIterator srcLast)
{
	typedef IteratorValue<TIterator> T;
	static_assert(IsSame<RemoveConst<IteratorValue<TIterator>>, T>);

	if constexpr (IsContiguousIterator<TSrcIterator> &&
		IsContiguousIterator<TIterator> && IsTriviallyCopyable<T>)
	{
		Memory::Copy(&*first, &*srcFirst, Ava_PTRSUB(&*srcLast, &*srcFirst));
	}
	else
	{
		Private::Algo_Utility::Copy_Iter(first, srcFirst, srcLast);
	}
}

template<typename TIterator, typename TSrcIterator>
Ava_FORCEINLINE void CopyFwd(TIterator first, TSrcIterator srcFirst, uword count)
{
	typedef IteratorValue<TIterator> T;
	static_assert(IsSame<RemoveConst<IteratorValue<TIterator>>, T>);

	if constexpr (IsContiguousIterator<TSrcIterator> &&
		IsContiguousIterator<TIterator> && IsTriviallyCopyable<T>)
	{
		Memory::Copy(&*first, &*srcFirst, count * sizeof(T));
	}
	else
	{
		if constexpr (IteratorCategory::Get<TSrcIterator> >= IteratorCategory::RandomAccess)
		{
			Private::Algo_Utility::Copy_Iter(first, srcFirst, srcFirst + count);
		}
		else
		{
			Private::Algo_Utility::Copy_Size(first, srcFirst, count);
		}
	}
}

} // namespace Ava::Algo
