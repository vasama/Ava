#pragma once

#include "Ava/Containers/Span.hpp"
#include "Ava/Containers/StoragePolicy.hpp"
#include "Ava/Algo/Utility.hpp"
#include "Ava/Memory/AlignedStorage.hpp"
#include "Ava/Memory/DefaultAllocator.hpp"
#include "Ava/Memory/PolymorphicAllocator.hpp"
#include "Ava/Meta/Constant.hpp"
#include "Ava/Meta/Identity.hpp"
#include "Ava/Meta/Traits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Private/Containers/NullAllocator.hpp"
#include "Ava/Private/Ebo.hpp"
#include "Ava/Utility/Forward.hpp"
#include "Ava/Utility/Move.hpp"

namespace Ava {

namespace Private::Containers_Array {

template<typename T, typename TStoragePolicy>
class StorageLayer
{
	static_assert(Identity<False, T>::Value, "Unsupported storage policy");
};


/* Local storage policy */

template<typename T, iword TCapacity>
struct Local_Base
{
	iword m_size;
	AlignedStorage<
		TCapacity * sizeof(T), alignof(T)
	> m_data;
};

template<typename T, iword TCapacity, bool = IsTriviallyDestructible<T>>
struct Local_DestroyLayer;

template<typename T, iword TCapacity>
struct Local_DestroyLayer<T, TCapacity, 0> : Local_Base<T, TCapacity>
{
	typedef Local_Base<T, TCapacity> Base;

	~Local_DestroyLayer()
	{
		if (iword size = Base::m_size)
			Algo::Destroy((T*)&this->Base::m_data, size);
	}
};

template<typename T, iword TCapacity>
struct Local_DestroyLayer<T, TCapacity, 1> : Local_Base<T, TCapacity>
{
};

template<typename T, iword TCapacity>
class StorageLayer<T, StoragePolicy::Local<TCapacity>>
	: public Private::Containers::NullAllocator
	, Local_DestroyLayer<T, TCapacity>
{
	typedef Local_Base<T, TCapacity> Base;

public:
	typedef Private::Containers::NullAllocator AllocatorType;

	StorageLayer(StorageLayer&& other)
	{
		iword size = other.m_size;
		if (size > 0)
		{
			Algo::RelocateFwd((T*)&this->Base::m_data,
				(T*)&other.m_data, size);
			other.m_size = 0;
		}
		Base::m_size = size;
	}

	StorageLayer(AllocatorType)
	{
		Base::m_size = 0;
	}

	StorageLayer(AllocatorType, StorageLayer&& other)
		: StorageLayer(Move(other))
	{
	}

	Ava_FORCEINLINE T* GetData() const
	{
		return (T*)&this->Base::m_data;
	}

	Ava_FORCEINLINE iword GetSize() const
	{
		return Base::m_size;
	}

	Ava_FORCEINLINE void SetSize(iword size)
	{
		Base::m_size = size;
	}

	Ava_FORCEINLINE iword GetCapacity() const
	{
		return Base::m_capacity;
	}

	Ava_FORCEINLINE void Expand(iword minCapacity)
	{
		Ava_Assert(minCapacity <= TCapacity);
	}

	Ava_FORCEINLINE void Shrink(iword minCapacity)
	{
		Ava_Assert(minCapacity <= TCapacity);
	}
};


/* Remote storage policy */

template<typename T>
struct Remote_Base
{
	T* m_data;
	iword m_size;
	iword m_capacity;
};

template<typename T, typename TAllocator>
class StorageLayer<T, StoragePolicy::Remote<TAllocator>>
	: Ava_EBOX(public, Remote_Base<T>, TAllocator)
{
	typedef Remote_Base<T> Base;
	typedef ConstParam<TAllocator> AllocatorParam;

public:
	typedef TAllocator AllocatorType;

	StorageLayer(AllocatorParam alloc)
		: TAllocator(alloc)
	{
		Base::m_size = 0;
		Base::m_capacity = 0;
	}

	StorageLayer(StorageLayer&& other)
		: TAllocator(other)
	{
		Adopt(other);
	}

	StorageLayer(AllocatorParam alloc, StorageLayer&& other)
		: TAllocator(alloc)
	{
		iword size = other.m_size;
		if (Ava_LIKELY(alloc == static_cast<const TAllocator&>(other)))
		{
			Adopt(other);
		}
		else if (size > 0)
		{
			T* otherData = other.m_data;
			iword otherCapacity = other.m_capacity;

			T* data = Allocate(otherCapacity);
			Algo::RelocateFwd(data, otherData, size);
			other.m_size = 0;

			Base::m_data = data;
			Base::m_size = size;
			Base::m_capacity = otherCapacity;
		}
	}

	~StorageLayer()
	{
		iword capacity = Base::m_capacity;
		if (capacity > 0)
		{
			T* data = Base::m_data;
			Algo::Destroy(data, Base::m_size);
			Deallocate(data, capacity);
		}
	}

	Ava_FORCEINLINE T* GetData() const
	{
		return Base::m_data;
	}

	Ava_FORCEINLINE iword GetSize() const
	{
		return Base::m_size;
	}

	Ava_FORCEINLINE void SetSize(iword size)
	{
		Base::m_size = size;
	}

	Ava_FORCEINLINE iword GetCapacity() const
	{
		return Base::m_capacity;
	}

	Ava_FORCEINLINE void Expand(iword minCapacity)
	{
		if (Ava_UNLIKELY(minCapacity > Base::m_capacity))
			ExpandInternal(minCapacity);
	}

	Ava_FORCEINLINE void Shrink(iword minCapacity)
	{
		iword newCapacity = minCapacity * GrowthFactor;
		if (newCapacity < Base::m_capacity)
			ShrinkInternal(minCapacity);
	}

	Ava_FORCEINLINE const TAllocator& GetAllocator() const
	{
		return static_cast<const TAllocator&>(*this);
	}

	Ava_FORCEINLINE void SetAllocator(const TAllocator& alloc)
	{
		static_cast<TAllocator&>(*this) = alloc;
	}

private:
	Ava_FORCEINLINE void Adopt(StorageLayer& other)
	{
		Base::m_data = other.m_data;
		Base::m_size = other.m_size;
		Base::m_capacity = other.m_capacity;

		other.m_size = 0;
		other.m_capacity = 0;
	}

	void Cleanup()
	{
		if (iword capacity = Base::m_capacity)
		{
			T* data = Base::m_data;
			Algo::Destroy(data, Base::m_size);
			Deallocate(data, capacity);
		}
	}

	Ava_FORCENOINLINE void ResizeInternal(iword capacity, iword newCapacity)
	{
		T* data = Base::m_data;

		T* newData = Allocate(newCapacity);
		Algo::RelocateFwd(newData, data, Base::m_size);
		Deallocate(data, capacity);

		Base::m_data = newData;
	}

	Ava_FORCENOINLINE void ExpandInternal(iword newCapacity)
	{
		iword capacity = Base::m_capacity;
		if (capacity == 0)
		{
			Base::m_data = Allocate(newCapacity);
		}
		else
		{
			iword grow = capacity * GrowthFactor;
			newCapacity = newCapacity > grow ? newCapacity : grow;
			ResizeInternal(capacity, newCapacity);
		}
		Base::m_capacity = newCapacity;
	}

	Ava_FORCENOINLINE void ShrinkInternal(iword newCapacity)
	{
		if (newCapacity == 0)
		{
			Deallocate(Base::m_data, Base::m_capacity);
		}
		else
		{
			ResizeInternal(Base::m_capacity, newCapacity);
		}
		Base::m_capacity = newCapacity;
	}

	Ava_FORCEINLINE T* Allocate(iword newCapacity) const
	{
		return (T*)TAllocator::Allocate(newCapacity * sizeof(T));
	}

	Ava_FORCEINLINE void Deallocate(T* data, iword capacity) const
	{
		TAllocator::Deallocate(data, capacity * sizeof(T));
	}

	static constexpr iword GrowthFactor = 2;

	static constexpr bool CanRelocate = IsTriviallyRelocatable<T>;
};

} // namespace Private::Containers_Array

template<typename T, typename TStoragePolicy>
class BasicArray : Private::Containers_Array::StorageLayer<T, TStoragePolicy>
{
	typedef Private::Containers_Array::StorageLayer<T, TStoragePolicy> Base;

public:
	typedef T ElementType;
	typedef typename Base::AllocatorType AllocatorType;

private:
	typedef ConstParam<AllocatorType> AllocatorParam;

public:
	typedef       T* Iterator;
	typedef const T* ConstIterator;

	Ava_FORCEINLINE BasicArray()
		: BasicArray(AllocatorType())
	{
	}

	explicit Ava_FORCEINLINE BasicArray(AllocatorParam alloc)
		: Base(alloc)
	{
	}

	Ava_FORCEINLINE BasicArray(BasicArray&& other)
		: Base(Move(other))
	{
	}

	Ava_FORCEINLINE BasicArray(AllocatorParam alloc, BasicArray&& other)
		: Base(alloc, Move(other))
	{
	}

	Ava_FORCEINLINE BasicArray(const BasicArray& other)
		: BasicArray(other.GetAllocator(), other)
	{
	}

	BasicArray(AllocatorParam alloc, const BasicArray& other)
		: Base(alloc)
	{
		iword size = other.Base::GetSize();
		if (size > 0)
		{
			Base::Expand(size);
			Algo::CopyConstruct(Base::GetData(), other.Base::GetData(), size);

			Base::SetSize(size);
		}
	}


	Ava_FORCEINLINE BasicArray& operator=(BasicArray&& other)
	{
		Assign(Move(other));
		return *this;
	}

	Ava_FORCEINLINE BasicArray& operator=(const BasicArray& other)
	{
		Assign(other);
		return *this;
	}

	Ava_FORCEINLINE void Assign(BasicArray&& other)
	{
		Assign(other.GetAllocator(), Move(other));
	}

	Ava_FORCEINLINE void Assign(AllocatorParam alloc, BasicArray&& other)
	{
		if (Ava_LIKELY(alloc == GetAllocator()))
		{
			Base::Assign(Move(other));
		}
		else
		{
			Base::Assign(alloc, Move(other));
		}
	}

	Ava_FORCEINLINE void Assign(const BasicArray& other)
	{
		Assign(other.GetAllocator(), other);
	}

	void Assign(AllocatorParam alloc, const BasicArray& other)
	{
		if (Ava_LIKELY(alloc == Base::GetAllocator()))
		{
			iword size = Base::GetSize();
			Algo::Destroy(Base::GetData(), size);
		}
		else
		{
			Base::Clean();
			Base::SetAllocator(alloc);
		}

		iword newSize = other.Base::GetSize();
		if (newSize > 0)
		{
			Base::Expand(newSize);
			Algo::CopyConstruct(Base::GetData(),
				other.Base::GetData(), newSize);
		}
		Base::SetSize(newSize);
	}


	Ava_FORCEINLINE iword Size() const
	{
		return Base::GetSize();
	}

	Ava_FORCEINLINE iword Capacity() const
	{
		return Base::GetCapacity();
	}


	Ava_FORCEINLINE T* Data()
	{
		return Base::GetData();
	}

	Ava_FORCEINLINE const T* Data() const
	{
		return Base::GetData();
	}

	Ava_FORCEINLINE T& operator[](iword index)
	{
		Ava_AssertSlow(CheckIndex(index));
		return Base::GetData()[index];
	}

	Ava_FORCEINLINE const T& operator[](iword index) const
	{
		Ava_AssertSlow(CheckIndex(index));
		return Base::GetData()[index];
	}


	Ava_FORCEINLINE Span<T> Slice(iword index)
	{
		Ava_AssertSlow(CheckIndex(index));
		return Span<T>(Base::GetData() + index, Base::GetSize() - index);
	}

	Ava_FORCEINLINE Span<const T> Slice(iword index) const
	{
		Ava_AssertSlow(CheckIndex(index));
		return Span<const T>(Base::GetData() + index, Base::GetSize() - index);
	}

	Ava_FORCEINLINE Span<T> Slice(iword index, iword count)
	{
		Ava_AssertSlow(CheckRange(index, count));
		return Span<T>(Base::GetData() + index, count);
	}

	Ava_FORCEINLINE Span<const T> Slice(iword index, iword count) const
	{
		Ava_AssertSlow(CheckRange(index, count));
		return Span<const T>(Base::GetData() + index, count);
	}

	Ava_FORCEINLINE operator Span<T>()
	{
		return Span<T>(Base::GetData(), Base::GetSize());
	}

	Ava_FORCEINLINE operator Span<const T>() const
	{
		return Span<const T>(Base::GetData(), Base::GetSize());
	}


	Ava_FORCEINLINE void Append(T&& value)
	{
		MoveConstruct<T>(AppendInternal(1), Move(value));
	}

	Ava_FORCEINLINE void Append(const T& value)
	{
		CopyConstruct<T>(AppendInternal(1), value);
	}

	Ava_FORCEINLINE void AppendRange(Span<T> span)
	{
		Algo::CopyConstruct(AppendInternal(span.Size()), span.Data(), span.Size());
	}

	Ava_FORCEINLINE void AppendRange(Span<const T> span)
	{
		Algo::CopyConstruct(AppendInternal(span.Size()), span.Data(), span.Size());
	}

	template<typename TRange>
	Ava_FORCEINLINE void AppendRange(const TRange& range)
	{
		Ava_UNUSED(range);
		Ava_Assert(false);
	}

	template<typename... TArgs>
	Ava_FORCEINLINE void AppendEmplace(TArgs&&... args)
	{
		Construct<T>(AppendInternal(1), Forward<TArgs>(args)...);
	}


	Ava_FORCEINLINE void Insert(iword index, T&& value)
	{
		MoveConstruct<T>(InsertInternal(index, 1), Move(value));
	}

	Ava_FORCEINLINE void Insert(iword index, const T& value)
	{
		CopyConstruct<T>(InsertInternal(index, 1), value);
	}

	Ava_FORCEINLINE void InsertRange(iword index, Span<T> span)
	{
		Algo::CopyConstruct(InsertInternal(index, span.Size()), span.Data(), span.Size());
	}

	Ava_FORCEINLINE void InsertRange(iword index, Span<const T> span)
	{
		Algo::CopyConstruct(InsertInternal(index, span.Size()), span.Data(), span.Size());
	}

	template<typename TRange>
	Ava_FORCEINLINE void InsertRange(iword index, const TRange& range)
	{
		Ava_UNUSED(index, range);
		Ava_Assert(false);
	}

	template<typename... TArgs>
	Ava_FORCEINLINE void InsertEmplace(iword index, TArgs&&... args)
	{
		Construct<T>(InsertInternal(index, 1), Forward<TArgs>(args)...);
	}


	Ava_FORCEINLINE void Remove(iword index)
	{
		RemoveInternal(index, 1, true);
	}

	Ava_FORCEINLINE void RemoveUnstable(iword index)
	{
		RemoveInternal(index, 1, false);
	}

	Ava_FORCEINLINE void RemoveRange(iword index, iword count)
	{
		RemoveInternal(index, count, true);
	}

	Ava_FORCEINLINE void RemoveRangeUnstable(iword index, iword count)
	{
		RemoveInternal(index, count, false);
	}


	Ava_FORCEINLINE T Extract(iword index)
	{
		T* elem = Base::GetData() + index;
		T result = Move(*elem);
		RemoveInternal(index, 1, true);
		return result;
	}

	Ava_FORCEINLINE T ExtractUnstable(iword index)
	{
		T* elem = Base::GetData() + index;
		T result = Move(*elem);
		RemoveInternal(index, 1, false);
		return result;
	}


	void Resize(iword newSize)
	{
		ResizeInternal(newSize, true);
	}

	void ResizeUnsafe(iword newSize)
	{
		ResizeInternal(newSize, false);
	}

	void Clear(bool deallocate = false)
	{
		iword size = Base::GetSize();
		if (size > 0)
			Algo::Destroy(Base::GetData(), size);
		Base::SetSize(0);

		if (deallocate)
			Base::Shrink(0);
	}

	void Reserve(iword newCapacity)
	{
		Base::Expand(newCapacity);
	}

	void ShrinkToFit()
	{
		Base::Shrink(Base::GetSize());
	}


	Ava_FORCEINLINE bool CheckIndex(iword index) const
	{
		return (uword)index < (uword)Base::GetSize();
	}

	Ava_FORCEINLINE bool CheckRange(iword index, iword count) const
	{
		iword size = Base::GetSize();
		return (uword)index < (uword)size &&
			(uword)count <= (uword)(size - index);
	}

	Ava_FORCEINLINE const AllocatorType& GetAllocator() const
	{
		return static_cast<const AllocatorType&>(*this);
	}

private:
	Ava_FORCEINLINE T* AppendInternal(iword count)
	{
		iword size = Base::GetSize();
		iword newSize = size + count;

		Base::Expand(newSize);

		T* elem = Base::GetData() + size;
		Base::SetSize(newSize);
		return elem;
	}

	Ava_FORCEINLINE T* InsertInternal(iword index, iword count)
	{
		iword size = Base::GetSize();
		iword newSize = size + count;

		Base::Expand(newSize);

		T* data = Base::GetData();

		T* rift = data + index;
		if (rift != data + size)
		{
			T* last = data + newSize;
			Algo::RelocateBwd(last, last - count, rift);
		}

		Base::SetSize(newSize);
		return rift;
	}

	Ava_FORCEINLINE void RemoveInternal(iword index, iword count, bool stable)
	{
		iword size = Base::GetSize();
		iword newSize = size - count;

		T* data = Base::GetData();

		T* rift = data + index;
		T* next = rift + count;

		Algo::Destroy(rift, next);

		T* last = data + size;

		if (rift < last - 1)
			Algo::RelocateFwd(rift, stable ? next : last - count, data + size);

		Base::SetSize(newSize);
	}

	Ava_FORCEINLINE void ResizeInternal(iword newSize, bool safe)
	{
		iword size = Base::GetSize();
		if (Ava_LIKELY(newSize > size))
		{
			Base::Expand(newSize);

			if (safe)
				Algo::ValueConstruct(Base::GetData() + size, newSize - size);

			Base::SetSize(newSize);
		}
		else if (Ava_LIKELY(newSize < size))
		{
			if (safe)
				Algo::Destroy(Base::GetData() + newSize, size - newSize);

			Base::SetSize(newSize);
		}
	}

	Ava_FORCEINLINE friend Iterator begin(BasicArray& array)
	{
		return array.Base::GetData();
	}

	Ava_FORCEINLINE friend Iterator end(BasicArray& array)
	{
		return array.Base::GetData() + array.Base::GetSize();
	}

	Ava_FORCEINLINE friend ConstIterator begin(const BasicArray& array)
	{
		return array.Base::GetData();
	}

	Ava_FORCEINLINE friend ConstIterator end(const BasicArray& array)
	{
		return array.Base::GetData() + array.Base::GetSize();
	}
};

template<typename T, typename TAllocator = DefaultAllocator>
using Array = BasicArray<T, StoragePolicy::Remote<TAllocator>>;

template<typename T>
using PmArray = Array<T, PolymorphicAllocator>;

template<typename T, iword TCapacity, typename TAllocator = DefaultAllocator>
using SmallArray = BasicArray<T, StoragePolicy::Small<TAllocator, TCapacity>>;

template<typename T, iword TCapacity>
using PmSmallArray = SmallArray<T, TCapacity, PolymorphicAllocator>;

template<typename T, iword TCapacity>
using InplaceArray = BasicArray<T, StoragePolicy::Local<TCapacity>>;

} // namespace Ava
