#pragma once

#include "Ava/Algo/Utility.hpp"
#include "Ava/Containers/Span.hpp"
#include "Ava/Debug/Assert.hpp"
#include "Ava/Debug/StaticAssert.hpp"
#include "Ava/Math/Math.hpp"
#include "Ava/Memory/AlignedStorage.hpp"
#include "Ava/Memory/DefaultAllocator.hpp"
#include "Ava/Memory/PolymorphicAllocator.hpp"
#include "Ava/Memory/Utility.hpp"
#include "Ava/Meta/Identity.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Private/Containers/NullAllocator.hpp"
#include "Ava/Private/Ebo.hpp"
#include "Ava/Private/PointerMath.hpp"
#include "Ava/Types.hpp"
#include "Ava/Utility/Forward.hpp"
#include "Ava/Utility/Move.hpp"
#include "Ava/Utility/Range.hpp"

namespace Ava {

#if Ava_IDE

template<typename T, typename TAllocator, iword TCapacity>
class BasicArray
{
public:
	typedef T ElementType;
	typedef TAllocator AllocatorType;

	typedef       T* Iterator;
	typedef const T* ConstIterator;

	BasicArray();
	explicit BasicArray(TAllocator);
	BasicArray(BasicArray&&);
	BasicArray(const BasicArray&) = delete;
	BasicArray& operator=(BasicArray&&);
	BasicArray& operator=(const BasicArray&) = delete;

	iword Size() const;
	iword Capacity() const;

	T* Data();
	const T* Data() const;
	const T* CData() const;
	T& operator[](iword index);
	const T& operator(iword index) const;
	T& First(iword index);
	const T& First(iword index) const;
	T& Last(iword index);
	const T& Last(iword index) const;

	Span<T> Slice(iword index);
	Span<const T> Slice(iword index) const;
	Span<T> Slice(iword index, iword count);
	Span<const T> Slice(iword index, iword count) const;
	Span<const T> CSlice(iword index) const;
	Span<const T> CSlice(iword index, iword count) const;
	operator Span<T>();
	operator Span<const T>() const;

	template<typename TIn>
	T* Append(TIn&& value);

	template<typename... TArgs>
	T* AppendEmplace(TArgs&&... args);

	void AppendRange(Span<T> span);
	void AppendRange(Span<const T> span);

	template<typename TRange>
	void AppendRange(TRange&& range);

	template<typename TIn>
	T* Insert(iword index, TIn&& value);

	template<typename... TArgs>
	T* InsertEmplace(iword index, TArgs&&... args);

	void InsertRange(iword index, Span<T> span);
	void InsertRange(iword index, Span<const T> span);

	template<typename TRange>
	void InsertRange(iword index, TRange&& range);

	void Remove(iword index);
	void RemoveUnstable(iword index);
	void RemoveRange(iword index, iword count);
	void RemoveRangeUnstable(iword index, iword count);
	T Extract(iword index);
	T ExtractUnstable(iword index);
	void Clear(bool deallocate = false);

	void Resize(iword newSize);
	void ResizeUnsafe(iword newSize);

	void Reserve(iword minCapacity);
	void ShrinkToFit();

	const TAllocator& GetAllocator() const;
};

template<typename T, typename TAllocator, iword TCapacity>
typename BasicArray<T, TAllocator, TCapacity>::Iterator begin(BasicArray<T, TAllocator, TCapacity>&);

template<typename T, typename TAllocator, iword TCapacity>
typename BasicArray<T, TAllocator, TCapacity>::ConstIterator begin(const BasicArray<T, TAllocator, TCapacity>&);

template<typename T, typename TAllocator, iword TCapacity>
typename BasicArray<T, TAllocator, TCapacity>::Iterator end(BasicArray<T, TAllocator, TCapacity>&);

template<typename T, typename TAllocator, iword TCapacity>
typename BasicArray<T, TAllocator, TCapacity>::ConstIterator end(const BasicArray<T, TAllocator, TCapacity>&);

#else

namespace Private::Containers_Array {

template<typename T>
struct Core
{
	T* m_beg;
	T* m_mid;
	T* m_end;
};

template<typename T, typename TAllocator, iword TCapacity>
struct StorageLayer : Ava_EBO(Core<T>, TAllocator)
{
	static constexpr bool MayAllocate = true;

	Ava_FORCEINLINE StorageLayer(TAllocator allocator)
		: TAllocator(Move(allocator))
	{
		Reset();
	}

	Ava_FORCEINLINE void Reset()
	{
		this->m_beg = (T*)&m_storage;
		this->m_mid = (T*)&m_storage;
		this->m_end = (T*)&m_storage + TCapacity;
	}

	Ava_FORCEINLINE T* Beg() const
	{
		return Core<T>::m_beg;
	}

	Ava_FORCEINLINE T* Mid() const
	{
		return Core<T>::m_mid;
	}

	Ava_FORCEINLINE T* End() const
	{
		return Core<T>::m_end;
	}

	Ava_FORCEINLINE bool IsDynamic(T* beg)
	{
		return beg != (T*)&m_storage;
	}

	AlignedStorage<
		sizeof(T) * TCapacity, alignof(T)
	> mutable m_storage;
};

template<typename T, typename TAllocator>
struct StorageLayer<T, TAllocator, 0> : Ava_EBO(Core<T>, TAllocator)
{
	static constexpr bool MayAllocate = true;

	Ava_FORCEINLINE StorageLayer(TAllocator allocator)
		: TAllocator(Move(allocator))
	{
		Reset();
	}

	Ava_FORCEINLINE void Reset()
	{
		this->m_beg = nullptr;
		this->m_mid = nullptr;
		this->m_end = nullptr;
	}

	Ava_FORCEINLINE T* Beg() const
	{
		return Core<T>::m_beg;
	}

	Ava_FORCEINLINE T* Mid() const
	{
		return Core<T>::m_mid;
	}

	Ava_FORCEINLINE T* End() const
	{
		return Core<T>::m_end;
	}

	Ava_FORCEINLINE bool IsDynamic(T* beg)
	{
		return beg != nullptr;
	}
};

template<typename T, iword TCapacity>
struct StorageLayer<T, Containers_::NullAllocator, TCapacity>
	: Containers_::NullAllocator
{
	Ava_StaticAssertSlow(TCapacity > 0);

	static constexpr bool MayAllocate = false;

	Ava_FORCEINLINE StorageLayer(Containers_::NullAllocator)
	{
		Reset();
	}

	Ava_FORCEINLINE void Reset()
	{
		this->m_mid = (T*)&m_storage;
	}

	Ava_FORCEINLINE T* Beg() const
	{
		return (T*)&m_storage;
	}

	Ava_FORCEINLINE T* Mid() const
	{
		return m_mid;
	}

	Ava_FORCEINLINE T* End() const
	{
		return (T*)&m_storage + TCapacity;
	}

	T* m_mid;

	AlignedStorage<
		sizeof(T) * TCapacity, alignof(T)
	> mutable m_storage;
};

} // namespace Private::Containers_Array

template<typename T, typename TAllocator, iword TCapacity>
class BasicArray
{
#define Ava_MOVEALLOC(...) static_cast<TAllocator&&>(__VA_ARGS__)
#define Ava_COPYALLOC(...) static_cast<const TAllocator&>(__VA_ARGS__)

	Ava_StaticAssertSlow(TCapacity >= 0);

public:
	typedef T ElementType;
	typedef TAllocator AllocatorType;

	typedef       T* Iterator;
	typedef const T* ConstIterator;


	Ava_FORCEINLINE BasicArray()
		: m_storage(TAllocator())
	{
	}

	explicit Ava_FORCEINLINE BasicArray(TAllocator allocator)
		: m_storage(Move(allocator))
	{
	}

	Ava_FORCEINLINE BasicArray(BasicArray&& src)
		: m_storage(Ava_MOVEALLOC(src.m_storage))
	{
		MoveConstructInternal(Move(src), TCapacity * sizeof(T));
	}

	template<iword TSrcCapacity>
	Ava_FORCEINLINE BasicArray(BasicArray<T, TAllocator, TSrcCapacity>&& src)
		: m_storage(Ava_MOVEALLOC(src.m_storage))
	{
		MoveConstructInternal(Move(src), TCapacity * sizeof(T));
	}

	BasicArray(const BasicArray&) = delete;

	Ava_FORCEINLINE BasicArray& operator=(BasicArray&& src)
	{
		MoveAssignInternal(Move(src), TCapacity * sizeof(T));
		return *this;
	}

	template<iword TSrcCapacity>
	Ava_FORCEINLINE BasicArray& operator=(BasicArray<T, TAllocator, TSrcCapacity>&& src)
	{
		MoveAssignInternal(Move(src), TCapacity * sizeof(T));
		return *this;
	}

	BasicArray& operator=(const BasicArray&) = delete;

	~BasicArray()
	{
		T* beg = m_storage.Beg();
		Algo::Destroy(beg, m_storage.m_mid);

		if constexpr (m_storage.MayAllocate)
			if (m_storage.IsDynamic(beg))
				m_storage.Deallocate(beg, Ava_PTRSUB(m_storage.m_end, beg));
	}


	Ava_FORCEINLINE iword Size() const
	{
		return m_storage.m_mid - m_storage.Beg();
	}

	Ava_FORCEINLINE iword Capacity() const
	{
		return m_storage.End() - m_storage.Beg();
	}


	Ava_FORCEINLINE T* Data()
	{
		return m_storage.Beg();
	}

	Ava_FORCEINLINE const T* Data() const
	{
		return m_storage.Beg();
	}

	Ava_FORCEINLINE const T* CData() const
	{
		return m_storage.Beg();
	}

	Ava_FORCEINLINE T& operator[](iword index)
	{
		Ava_Assert(CheckIndex(index));
		return *(m_storage.Beg() + index);
	}

	Ava_FORCEINLINE const T& operator[](iword index) const
	{
		Ava_Assert(CheckIndex(index));
		return *(m_storage.Beg() + index);
	}

	Ava_FORCEINLINE T& First()
	{
		Ava_Assert(m_storage.m_mid != m_storage.End());
		return *m_storage.Beg();
	}

	Ava_FORCEINLINE const T& First() const
	{
		Ava_Assert(m_storage.m_mid != m_storage.End());
		return *m_storage.Beg();
	}

	Ava_FORCEINLINE T& Last()
	{
		Ava_Assert(m_storage.m_mid != m_storage.End());
		return *(m_storage.m_mid - 1);
	}

	Ava_FORCEINLINE const T& Last() const
	{
		Ava_Assert(m_storage.m_mid != m_storage.End());
		return *(m_storage.m_mid - 1);
	}


	Ava_FORCEINLINE Span<T> Slice(iword index)
	{
		Ava_Assert(CheckIndex(index));
		return Span<T>(m_storage.Beg() + index, m_storage.m_mid);
	}

	Ava_FORCEINLINE Span<const T> Slice(iword index) const
	{
		Ava_Assert(CheckIndex(index));
		return Span<const T>(m_storage.Beg() + index, m_storage.m_mid);
	}

	Ava_FORCEINLINE Span<T> Slice(iword index, iword count)
	{
		Ava_Assert(CheckRange(index, count));
		T* data = m_storage.Beg() + index;
		return Span<T>(data, data + count);
	}

	Ava_FORCEINLINE Span<const T> Slice(iword index, iword count) const
	{
		Ava_Assert(CheckRange(index, count));
		T* data = m_storage.Beg() + index;
		return Span<const T>(data, data + count);
	}

	Ava_FORCEINLINE Span<const T> CSlice(iword index) const
	{
		return Slice(index);
	}

	Ava_FORCEINLINE Span<const T> CSlice(iword index, iword count) const
	{
		return Slice(index, count);
	}

	Ava_FORCEINLINE operator Span<T>()
	{
		return Span<T>(m_storage.Beg(), m_storage.m_mid);
	}

	Ava_FORCEINLINE operator Span<const T>() const
	{
		return Span<const T>(m_storage.Beg(), m_storage.m_mid);
	}


	template<typename TIn>
	Ava_FORCEINLINE T* Append(TIn&& value)
	{
		T* slot = AppendInternal(1);
		Construct<T>(slot, Forward<TIn>(value));
		return slot;
	}

	template<typename... TArgs>
	Ava_FORCEINLINE T* AppendEmplace(TArgs&&... args)
	{
		T* slot = AppendInternal(1);
		Construct<T>(slot, Forward<TArgs>(args)...);
		return slot;
	}

	Ava_FORCEINLINE void AppendRange(Span<T> span)
	{
		Algo::CopyConstruct(AppendInternal(
			span.Size()), span.Begin(), span.End());
	}

	Ava_FORCEINLINE void AppendRange(Span<const T> span)
	{
		Algo::CopyConstruct(AppendInternal(
			span.Size()), span.Begin(), span.End());
	}

	template<typename TRange>
	Ava_FORCEINLINE void AppendRange(TRange&& range)
	{
		T* area = AppendInternal(Ava::Size(range));

		if constexpr (IsRRef<TRange>)
		{
			Algo::ConvMoveConstruct(area,
				Ava::Begin(range), Ava::End(range));
		}
		else
		{
			Algo::ConvCopyConstruct(area,
				Ava::Begin(range), Ava::End(range));
		}
	}


	template<typename TIn>
	Ava_FORCEINLINE T* Insert(iword index, TIn&& value)
	{
		Ava_Assert(CheckRange(index, 0));
		T* slot = InsertInternal(index, 1);
		Construct<T>(slot, Forward<TIn>(value));
		return slot;
	}

	template<typename... TArgs>
	Ava_FORCEINLINE T* InsertEmplace(iword index, TArgs&&... args)
	{
		Ava_Assert(CheckRange(index, 0));
		T* slot = InsertInternal(index, 1);
		Construct<T>(slot, Forward<TArgs>(args)...);
		return slot;
	}

	Ava_FORCEINLINE void InsertRange(iword index, Span<T> span)
	{
		Ava_Assert(CheckRange(index, 0));
		Algo::CopyConstruct(InsertInternal(index,
			span.Size()), span.Begin(), span.End());
	}

	Ava_FORCEINLINE void InsertRange(iword index, Span<const T> span)
	{
		Ava_Assert(CheckRange(index, 0));
		Algo::CopyConstruct(InsertInternal(index,
			span.Size()), span.Begin(), span.End());
	}

	template<typename TRange>
	Ava_FORCEINLINE void InsertRange(iword index, TRange&& range)
	{
		Ava_Assert(CheckRange(index, 0));
		T* area = InsertInternal(index, Ava::Size(range));

		if constexpr (IsRRef<TRange>)
		{
			Algo::ConvMoveConstruct(area,
				Ava::Begin(range), Ava::End(range));
		}
		else
		{
			Algo::ConvCopyConstruct(area,
				Ava::Begin(range), Ava::End(range));
		}
	}


	Ava_FORCEINLINE void Remove(iword index)
	{
		Ava_Assert(CheckIndex(index));
		RemoveInternal(index, 1, true);
	}

	Ava_FORCEINLINE void RemoveUnstable(iword index)
	{
		Ava_Assert(CheckIndex(index));
		RemoveInternal(index, 1, false);
	}

	Ava_FORCEINLINE void RemoveRange(iword index, iword count)
	{
		Ava_Assert(CheckRange(index, count));
		RemoveInternal(index, count, true);
	}

	Ava_FORCEINLINE void RemoveRangeUnstable(iword index, iword count)
	{
		Ava_Assert(CheckRange(index, count));
		RemoveInternal(index, count, false);
	}

	Ava_FORCEINLINE T Extract(iword index)
	{
		Ava_Assert(CheckIndex(index));
		T result = Move(*(m_storage.Beg() + index));
		RemoveInternal(index, 1, true);
		return result;
	}

	Ava_FORCEINLINE T ExtractUnstable(iword index)
	{
		Ava_Assert(CheckIndex(index));
		T result = Move(*(m_storage.Beg() + index));
		RemoveInternal(index, 1, false);
		return result;
	}

	Ava_FORCEINLINE void Clear()
	{
		T* beg = m_storage.Beg();
		Algo::Destroy(beg, m_storage.m_mid);
		m_storage.m_mid = beg;
	}


	Ava_FORCEINLINE void Resize(iword newSize)
	{
		ResizeInternal(newSize * sizeof(T), true);
	}

	Ava_FORCEINLINE void ResizeUnsafe(iword newSize)
	{
		ResizeInternal(newSize * sizeof(T), false);
	}


	void Reserve(iword minCapacity)
	{
		uword minByteCapacity = minCapacity * sizeof(T);
		uword byteCapacity = Ava_PTRSUB(m_storage.End(), m_storage.Beg());
		if (Ava_LIKELY(minByteCapacity > byteCapacity))
			ExpandInternal(minByteCapacity);
	}

	void ShrinkToFit()
	{
		Shrink(m_storage.m_mid);
	}


	const TAllocator& GetAllocator() const
	{
		return Ava_COPYALLOC(m_storage);
	}

private:
	template<iword TSrcCapacity>
	void MoveConstructInternal(BasicArray<T, TAllocator, TSrcCapacity>&& src, uword localByteCapacity)
	{
		T* srcbeg = src.m_storage.Beg();
		T* srcmid = src.m_storage.m_mid;
		uword byteSize = Ava_PTRSUB(srcmid, srcbeg);

		if constexpr (m_storage.MayAllocate)
		{
			if (!src.m_storage.IsDynamic(src.m_storage.m_beg))
				goto move_elements;

			if constexpr (TCapacity > TSrcCapacity)
				if (Ava_PTRSUB(src.m_storage.m_end, src.m_storage.m_beg) < localByteCapacity)
					goto move_elements;

			m_storage.m_beg = src.m_storage.m_beg;
			m_storage.m_mid = src.m_storage.m_mid;
			m_storage.m_end = src.m_storage.m_end;
			src.m_storage.Reset();

			return;
		move_elements:

			if (byteSize > localByteCapacity)
			{
				T* beg = (T*)m_storage.Allocate(byteSize);
				T* end = Ava_PTRADD(beg, byteSize);

				m_storage.m_beg = beg;
				m_storage.m_mid = end;
				m_storage.m_end = end;
			}
		}
		else Ava_Assert(byteSize <= localByteCapacity);

		T* beg = m_storage.Beg();
		Algo::Relocate(beg, srcbeg, srcmid);
		m_storage.m_mid = Ava_PTRADD(beg, byteSize);
		src.m_storage.m_mid = srcbeg;
	}

	template<iword TSrcCapacity>
	void MoveAssignInternal(BasicArray<T, TAllocator, TSrcCapacity>&& src, uword localByteCapacity)
	{
		T* srcbeg = src.m_storage.Beg();
		T* srcmid = src.m_storage.m_mid;
		uword byteSize = Ava_PTRSUB(srcmid, srcbeg);

		T* beg = m_storage.Beg();
		T* mid = m_storage.m_mid;
		Algo::Destroy(beg, mid);

		mid = Ava_PTRADD(beg, byteSize);
		if constexpr (m_storage.MayAllocate)
		{
			if (Ava_UNLIKELY(Ava_COPYALLOC(src.m_storage) != Ava_COPYALLOC(m_storage)))
				goto move_elements;

			if (!src.m_storage.IsDynamic(src.m_storage.m_beg))
				goto move_elements;

			if constexpr (TCapacity > TSrcCapacity)
			{
				if (Ava_PTRSUB(src.m_storage.m_end, src.m_storage.m_beg) < localByteCapacity)
					goto move_elements;
			}
			else Ava_UNUSED(localByteCapacity);

			if (m_storage.IsDynamic(beg))
				m_storage.Deallocate(beg, Ava_PTRSUB(m_storage.m_end, beg));

			m_storage.m_beg = src.m_storage.m_beg;
			m_storage.m_mid = src.m_storage.m_mid;
			m_storage.m_end = src.m_storage.m_end;
			src.m_storage.Reset();

			return;
		move_elements:

			if (mid > m_storage.m_end)
			{
				beg = ExpandInternal(byteSize);
				mid = Ava_PTRADD(beg, byteSize);
			}
		}
		else Ava_Assert(byteSize <= localByteCapacity);

		Algo::Relocate(beg, srcbeg, srcmid);
		src.m_storage.m_mid = srcbeg;
		m_storage.m_mid = mid;
	}

	Ava_FORCEINLINE T* AppendInternal(iword count)
	{
		T* mid = m_storage.m_mid;
		T* newmid = mid + count;

		if constexpr (m_storage.MayAllocate)
		{
			if (Ava_UNLIKELY(newmid > m_storage.m_end))
			{
				T* beg = m_storage.Beg();

				uword byteSize = Ava_PTRSUB(newmid, beg);
				T* newbeg = ExpandInternal(byteSize);

				mid = Ava_PTRADD(newbeg, Ava_PTRSUB(mid, beg));
				newmid = Ava_PTRADD(newbeg, byteSize);
			}
		}
		else Ava_Assert(newmid - m_storage.Beg() <= TCapacity);

		m_storage.m_mid = newmid;
		return mid;
	}

	T* InsertInternal(iword index, iword count)
	{
		T* beg = m_storage.Beg();
		T* mid = m_storage.m_mid;
		T* newmid = mid + count;

		if constexpr (m_storage.MayAllocate)
		{
			if (Ava_UNLIKELY(newmid > m_storage.m_end))
			{
				uword byteSize = Ava_PTRSUB(newmid, beg);
				T* newbeg = ExpandInternal(byteSize);

				mid = Ava_PTRADD(newbeg, Ava_PTRSUB(mid, beg));
				newmid = Ava_PTRADD(newbeg, byteSize);
				beg = newbeg;
			}
		}
		else Ava_Assert(newmid - beg <= TCapacity);

		T* rift = beg + index;

		if (rift != mid)
			Algo::RelocateBwd(newmid, mid, rift);

		m_storage.m_mid = newmid;
		return rift;
	}

	void RemoveInternal(iword index, iword count, bool stable)
	{
		T* beg = m_storage.Beg();
		T* mid = m_storage.m_mid;

		T* rift = beg + index;
		T* next = rift + count;

		Algo::Destroy(rift, next);

		T* newmid = mid - count;

		if (next != mid)
			Algo::RelocateFwd(rift, stable ? next : newmid, mid);

		m_storage.m_mid = newmid;
	}

	void ResizeInternal(uword byteSize, bool safe)
	{
		T* beg = m_storage.Beg();
		T* mid = m_storage.m_mid;
		T* newmid = Ava_PTRADD(beg, byteSize);

		if (Ava_LIKELY(newmid > mid))
		{
			if constexpr (m_storage.MayAllocate)
			{
				T* newbeg = ExpandInternal(byteSize);
				mid = Ava_PTRADD(newbeg, Ava_PTRSUB(mid, beg));
				newmid = Ava_PTRADD(newbeg, byteSize);
			}
			else Ava_Assert(newmid - beg <= TCapacity);

			if (safe)
				Algo::ValueConstruct(mid, newmid);
		}
		else if (Ava_LIKELY(newmid < mid))
		{
			if (safe)
				Algo::Destroy(newmid, mid);
		}
		m_storage.m_mid = newmid;
	}

	Ava_FORCENOINLINE T* ExpandInternal(uword minByteCapacity)
	{
		T* beg = m_storage.m_beg;
		T* mid = m_storage.m_mid;
		T* end = m_storage.m_end;

		uword byteCapacity = Ava_PTRSUB(end, beg);
		uword newByteCapacity = Math::Max(
			minByteCapacity, byteCapacity * 2);

		T* newbeg = (T*)m_storage.Allocate(newByteCapacity);
		T* newend = Ava_PTRADD(newbeg, newByteCapacity);

		if (beg != mid)
			Algo::Relocate(newbeg, beg, mid);

		if (m_storage.IsDynamic(beg))
			m_storage.Deallocate(beg, byteCapacity);

		m_storage.m_beg = newbeg;
		m_storage.m_mid = Ava_PTRADD(newbeg, Ava_PTRSUB(mid, beg));
		m_storage.m_end = newend;

		return newbeg;
	}

	Ava_FORCEINLINE bool CheckIndex(iword index) const
	{
		return (uword)index < (uword)Size();
	}

	Ava_FORCEINLINE bool CheckRange(iword index, iword count) const
	{
		iword size = Size();
		return (uword)index <= (uword)size &&
			(uword)count <= (uword)(size - index);
	}

	Private::Containers_Array::StorageLayer<
		T, TAllocator, TCapacity> m_storage;

	Ava_FORCEINLINE friend Iterator begin(BasicArray& array)
	{
		return array.m_storage.Beg();
	}

	Ava_FORCEINLINE friend ConstIterator begin(const BasicArray& array)
	{
		return array.m_storage.Beg();
	}

	Ava_FORCEINLINE friend Iterator end(BasicArray& array)
	{
		return array.m_storage.m_mid;
	}

	Ava_FORCEINLINE friend ConstIterator end(const BasicArray& array)
	{
		return array.m_storage.m_mid;
	}

#undef Ava_MOVEALLOC
#undef Ava_COPYALLOC
};

#endif

template<typename T, typename TAllocator, iword TCapacity>
Ava_FORCEINLINE iword Ava_Ext_Size(const BasicArray<T, TAllocator, TCapacity>& array)
{
	return array.Size();
}


template<typename T, typename TAllocator = DefaultAllocator>
using Array = BasicArray<T, TAllocator, 0>;

template<typename T>
using PmArray = Array<T, PolymorphicAllocator>;

template<typename T, iword TCapacity, typename TAllocator = DefaultAllocator>
using SmallArray = BasicArray<T, TAllocator, TCapacity>;

template<typename T, iword TCapacity>
using PmSmallArray = SmallArray<T, TCapacity, PolymorphicAllocator>;

template<typename T, iword TCapacity>
using InplaceArray = BasicArray<T, Private::Containers_::NullAllocator, TCapacity>;

} // namespace Ava
