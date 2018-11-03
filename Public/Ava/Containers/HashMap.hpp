#pragma once

#include "Ava/Memory/DefaultAllocator.hpp"
#include "Ava/Memory/PolymorphicAllocator.hpp"
#include "Ava/Meta/EnableIf.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Containers/KeyValuePair.hpp"
#include "Ava/Containers/StoragePolicy.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Private/Containers/HashTable.hpp"
#include "Ava/Private/Containers/MapTraits.hpp"
#include "Ava/Types.hpp"
#include "Ava/Utility/Default.hpp"
#include "Ava/Utility/Forward.hpp"
#include "Ava/Utility/Hash.hpp"
#include "Ava/Utility/Key.hpp"
#include "Ava/Utility/Move.hpp"

namespace Ava {

template<typename TKey, typename TValue, typename THasher, typename TStoragePolicy>
class BasicHashMap
{
	typedef Private::Containers_HashTable::Table<
		KeyValuePair<TKey, TValue>, Private::Containers::MapTraits,
		SelectDefault<THasher, SelectHasher<TKey>>, TStoragePolicy> Table;

public:
	typedef TKey KeyType;
	typedef TValue ValueType;

	typedef KeyValuePair<TKey, TValue> ElementType;
	typedef typename Table::AllocatorType AllocatorType;
	typedef SelectDefault<THasher, SelectHasher<TKey>> HasherType;

	typedef typename Table::Iterator Iterator;
	typedef typename Table::ConstIterator ConstIterator;

	typedef typename Table::InsertResult InsertResult;

	Ava_FORCEINLINE BasicHashMap()
		: m_table(AllocatorType(), HasherType())
	{
	}

	explicit Ava_FORCEINLINE BasicHashMap(HasherType hasher)
		: m_table(AllocatorType(), Move(hasher))
	{
	}

	explicit Ava_FORCEINLINE BasicHashMap(const AllocatorType& alloc)
		: m_table(alloc, HasherType())
	{
	}

	Ava_FORCEINLINE BasicHashMap(const AllocatorType& alloc, HasherType hasher)
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

#define Ava_SFINAE(...) EnableIf<IsSameKeyClass<TKey, TInKey>, __VA_ARGS__>

	template<typename TInKey>
	Ava_FORCEINLINE Ava_SFINAE(bool) Contains(const TInKey& key) const
	{
		return m_table.Find(key) != nullptr;
	}

	template<typename TInKey>
	Ava_FORCEINLINE Ava_SFINAE(ElementType*) Find(const TInKey& key)
	{
		return m_table.Find(key);
	}

	template<typename TInKey>
	Ava_FORCEINLINE Ava_SFINAE(const ElementType*) Find(const TInKey& key) const
	{
		return m_table.Find(key);
	}

	template<typename TInKey>
	Ava_FORCEINLINE Ava_SFINAE(TValue&) operator[](const TInKey& key)
	{
		ElementType* element = m_table.Find(key);
		Ava_Assert(element != nullptr);
		return element->Value;
	}

	template<typename TInKey>
	Ava_FORCEINLINE Ava_SFINAE(const TValue&) operator[](const TInKey& key) const
	{
		ElementType* element = m_table.Find(key);
		Ava_Assert(element != nullptr);
		return element->Value;
	}

	template<typename TInKey>
	Ava_FORCEINLINE Ava_SFINAE(bool) Remove(const TInKey& key)
	{
		return m_table.Remove(key);
	}

#undef Ava_SFINAE
#define Ava_SFINAE(...) EnableIf<IsSameKeyClass<TKey, Decay<TInKey>>, __VA_ARGS__>

	template<typename TInKey>
	Ava_FORCEINLINE Ava_SFINAE(InsertResult) Insert(TInKey&& key)
	{
		InsertResult result = m_table.Insert(key);
		if (result.Inserted)
			Construct<ElementType>(result.Element, Forward<TInKey>(key));
		return result;
	}

#undef Ava_SFINAE
#define Ava_SFINAE(...) EnableIf<IsSameKeyClass<TKey, Decay<TInKey>> && HasConv<TInValue, TValue>, __VA_ARGS__>

	template<typename TInKey, typename TInValue>
	Ava_FORCEINLINE Ava_SFINAE(InsertResult) Insert(TInKey&& key, TInValue&& value)
	{
		InsertResult result = m_table.Insert(key);
		if (result.Inserted)
			Construct<ElementType>(result.Element,
				Forward<TInKey>(key), Forward<TInValue>(value));
		return result;
	}

	template<typename TInKey, typename TInValue>
	Ava_FORCEINLINE Ava_SFINAE(InsertResult) Set(TInKey&& key, TInValue&& value)
	{
		InsertResult result = m_table.Insert(key);
		if (result.Inserted)
			Construct<ElementType>(result.Element,
				Forward<TInKey>(key), Forward<TInValue>(value));
		else
			result.Element->Value = Forward<TInValue>(value);
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

template<typename TKey, typename TValue, typename TAllocator = Default, typename THasher = Default>
using HashMap = BasicHashMap<TKey, TValue, THasher, StoragePolicy::Remote<SelectDefault<TAllocator, DefaultAllocator>>>;

template<typename TKey, typename TValue, typename THasher = Default>
using PmHashMap = HashMap<TKey, TValue, PolymorphicAllocator, THasher>;

template<typename TKey, typename TValue, iword TCapacity, typename TAllocator = Default, typename THasher = Default>
using SmallHashMap = BasicHashMap<TKey, TValue, THasher, StoragePolicy::Small<SelectDefault<TAllocator, DefaultAllocator>, TCapacity>>;

template<typename TKey, typename TValue, iword TCapacity, typename THasher = Default>
using PmSmallHashMap = SmallHashMap<TKey, TValue, TCapacity, PolymorphicAllocator, THasher>;

template<typename TKey, typename TValue, iword TCapacity, typename THasher = Default>
using InplaceHashMap = BasicHashMap<TKey, TValue, THasher, StoragePolicy::Local<TCapacity>>;

} // namespace Ava
