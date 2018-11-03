#pragma once

#include "Ava/Memory/DefaultAllocator.hpp"
#include "Ava/Memory/PolymorphicAllocator.hpp"
#include "Ava/Meta/EnableIf.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Containers/StoragePolicy.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Private/Containers/HashTable.hpp"
#include "Ava/Private/Containers/SetTraits.hpp"
#include "Ava/Types.hpp"
#include "Ava/Utility/Default.hpp"
#include "Ava/Utility/Forward.hpp"
#include "Ava/Utility/Hash.hpp"
#include "Ava/Utility/Key.hpp"
#include "Ava/Utility/Move.hpp"

namespace Ava {

template<typename T, typename THasher, typename TStoragePolicy>
class BasicHashSet
{
	typedef Private::Containers_HashTable::Table<T, Private::Containers::SetTraits,
		SelectDefault<THasher, SelectHasher<T>>, TStoragePolicy> Table;

public:
	typedef T ElementType;
	typedef typename Table::AllocatorType AllocatorType;
	typedef SelectDefault<THasher, SelectHasher<T>> HasherType;

	typedef typename Table::Iterator Iterator;
	typedef typename Table::ConstIterator ConstIterator;

	typedef typename Table::InsertResult InsertResult;

	Ava_FORCEINLINE BasicHashSet()
		: m_table(AllocatorType(), HasherType())
	{
	}

	explicit Ava_FORCEINLINE BasicHashSet(HasherType hasher)
		: m_table(AllocatorType(), Move(hasher))
	{
	}

	explicit Ava_FORCEINLINE BasicHashSet(const AllocatorType& alloc)
		: m_table(alloc, HasherType())
	{
	}

	Ava_FORCEINLINE BasicHashSet(const AllocatorType& alloc, HasherType hasher)
		: m_table(alloc, Move(hasher))
	{
	}

	Ava_FORCEINLINE iword Size() const
	{
		return m_table.GetSize();
	}

	Ava_FORCEINLINE iword Capacity() const
	{
		return m_table.GetCapacity();
	}

#define Ava_SFINAE(...) EnableIf<IsSameKeyClass<T, TIn>, __VA_ARGS__>

	template<typename TIn>
	Ava_FORCEINLINE Ava_SFINAE(bool) Contains(const TIn& value)
	{
		return m_table.Find(value) != nullptr;
	}

	template<typename TIn>
	Ava_FORCEINLINE Ava_SFINAE(ElementType*) Find(const TIn& value)
	{
		return m_table.Find(value);
	}

	template<typename TIn>
	Ava_FORCEINLINE Ava_SFINAE(const ElementType*) Find(const TIn& value) const
	{
		return m_table.Find(value);
	}

	template<typename TIn>
	Ava_FORCEINLINE Ava_SFINAE(T&) operator[](const TIn& value)
	{
		ElementType* element = m_table.Find(value);
		Ava_Assert(element != nullptr);
		return *element;
	}

	template<typename TIn>
	Ava_FORCEINLINE Ava_SFINAE(const T&) operator[](const TIn& value) const
	{
		ElementType* element = m_table.Find(value);
		Ava_Assert(element != nullptr);
		return *element;
	}

	template<typename TIn>
	Ava_FORCEINLINE Ava_SFINAE(bool) Remove(const TIn& value)
	{
		return m_table.Remove(value);
	}

#undef Ava_SFINAE
#define Ava_SFINAE(...) EnableIf<IsSameKeyClass<T, Decay<TIn>>, __VA_ARGS__>

	template<typename TIn>
	Ava_FORCEINLINE Ava_SFINAE(InsertResult) Insert(TIn&& value)
	{
		InsertResult result = m_table.Insert(value);
		if (result.Inserted)
			Construct<ElementType>(result.Element, Forward<TIn>(value));
		return result;
	}

#undef Ava_SFINAE

	Ava_FORCEINLINE void Remove(Iterator iterator)
	{
		Ava_UNUSED(iterator);
		Ava_Assert(false);
	}

	Ava_FORCEINLINE void Remove(ConstIterator iterator)
	{
		Ava_UNUSED(iterator);
		Ava_Assert(false);
	}

	Ava_FORCEINLINE void Reserve(iword minCapacity)
	{
		m_table.Reserve(minCapacity);
	}

	Ava_FORCEINLINE void Clear(bool deallocate = false)
	{
		m_table.Clear(deallocate);
	}

private:
	Table m_table;
};

template<typename T, typename TAllocator = Default, typename THasher = Default>
using HashSet = BasicHashSet<T, THasher, StoragePolicy::Remote<SelectDefault<TAllocator, DefaultAllocator>>>;

template<typename T, typename THasher = Default>
using PmHashSet = HashSet<T, PolymorphicAllocator, THasher>;

template<typename T, iword TCapacity, typename TAllocator = Default, typename THasher = Default>
using SmallHashSet = BasicHashSet<T, THasher, StoragePolicy::Small<SelectDefault<TAllocator, DefaultAllocator>, TCapacity>>;

template<typename T, iword TCapacity, typename THasher = Default>
using PmSmallHashSet = SmallHashSet<T, TCapacity, PolymorphicAllocator, THasher>;

template<typename T, iword TCapacity, typename THasher = Default>
using InplaceHashSet = BasicHashSet<T, THasher, StoragePolicy::Local<TCapacity>>;

} // namespace Ava
