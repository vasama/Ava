#pragma once

#include "Ava/Containers/StoragePolicy.hpp"
#include "Ava/Math/Math.hpp"
#include "Ava/Memory/NoInit.hpp"
#include "Ava/Memory/Storage.hpp"
#include "Ava/Meta/Constant.hpp"
#include "Ava/Meta/Identity.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Private/Ebo.hpp"
#include "Ava/Types.hpp"
#include "Ava/Utility/Hash.hpp"

namespace Ava::Private::Containers_HashTable {

enum class Ctrl : byte
{
	Null = 0b00000000,
	Last = 0b01111111,

	Flag = 0b10000000,
	Mask = 0b01111111,
};

constexpr Ava_FORCEINLINE Ctrl operator|(Ctrl lhs, Ctrl rhs)
{
	return (Ctrl)((byte)lhs | (byte)rhs);
}

constexpr Ava_FORCEINLINE Ctrl operator&(Ctrl lhs, Ctrl rhs)
{
	return (Ctrl)((byte)lhs & (byte)rhs);
}

extern const uword JumpOffsets[];

template<typename T>
struct Block
{
	//TODO: calculate optimal size
	static constexpr iword Size = 8;

	Ctrl m_ctrl[Size];
	Storage<T> m_data[Size];
};

constexpr iword MaxBlockSize = 16;
extern const Ctrl EmptyBlock[];

template<typename T>
void Clean(Block<T>* blocks, iword blockCount)
{
	if constexpr (!IsTriviallyDestructible<T>)
	{
		for (Block<T>* f = blocks, *l = f + blockCount; f != l; ++f)
			for (iword index = 0; index < Block<T>::Size; ++index)
				if (f->m_ctrl[index] != Ctrl::Null)
					f->m_data[index].Destroy();
	}
	else Ava_UNUSED(blocks, blockCount);
}


template<typename T>
struct IteratorBase
{
	typedef T ElementType;
	typedef Block<T> BlockType;

	ElementType* m_element;
	BlockType* m_block;
};

template<typename T>
class BasicIterator : IteratorBase<RemoveConst<T>>
{
	typedef IteratorBase<RemoveConst<T>> Base;

public:
	Ava_FORCEINLINE BasicIterator(decltype(NoInit))
	{
	}

	Ava_FORCEINLINE T& operator*() const
	{
		return *Base::m_element;
	}

	Ava_FORCEINLINE T* operator->() const
	{
		return Base::m_element;
	}

	Ava_FORCEINLINE BasicIterator& operator++()
	{
		IncrementInternal();
		return *this;
	}

	Ava_FORCEINLINE BasicIterator operator++(int)
	{
		BasicIterator result = *this;
		IncrementInternal();
		return result;
	}

private:
	void IncrementInternal()
	{
		typename Base::ElementType* element = Base::m_element;
		typename Base::BlockType* block = Base::m_block;

		while (true)
		{
			Ctrl* first = block->m_ctrl + (element - &block->m_data);
			Ctrl* last = block->m_ctrl + Base::BlockType::Size;

			for (Ctrl* ctrl = first; ++ctrl != last;)
				if (*ctrl == Ctrl::Null)
				{
					Base::m_element = element + (ctrl - first);
					Base::m_block = block;
					return;
				}

			element = &++block->m_data;
		}
	}

	friend Ava_FORCEINLINE bool operator==(
		const BasicIterator<T>& lhs, const BasicIterator<T>& rhs)
	{
		return lhs.m_element == rhs.m_element && lhs.m_block == rhs.m_block;
	}

	friend Ava_FORCEINLINE bool operator!=(
		const BasicIterator<T>& lhs, const BasicIterator<T>& rhs)
	{
		return lhs.m_element != rhs.m_element || lhs.m_block != rhs.m_block;
	}
};


template<typename T, typename THasher, typename TStoragePolicy>
class StorageLayer
{
	static_assert(Identity<False, T>::Value, "Unsupported storage policy");
};


template<typename TBase, typename THasher>
struct HasherLayer : Ava_EBO(TBase, THasher)
{
	template<typename TInHasher>
	Ava_FORCEINLINE HasherLayer(TInHasher&& hasher)
		: THasher(Forward<TInHasher>(hasher))
	{
	}
};


/* Remote storage policy */

template<typename TBlock>
struct Remote_Base
{
	TBlock* m_blocks;

	u32 m_size;
	u32 m_capacity;
};

template<typename TBlock, typename THasher, typename TAllocator>
class StorageLayer<TBlock, THasher, StoragePolicy::Remote<TAllocator>>
	: Ava_EBOX(public, HasherLayer<Remote_Base<TBlock>, THasher>, TAllocator)
{
	typedef Remote_Base<TBlock> Base;
	typedef HasherLayer<Base, THasher> HasherLayer;

public:
	typedef TAllocator AllocatorType;
	static constexpr bool CanGrow = true;

	Ava_CLANG_WARNING(push)
	Ava_CLANG_WARNING(ignored "-Wreorder")
	template<typename TInHasher>
	Ava_FORCEINLINE StorageLayer(const AllocatorType& alloc, TInHasher&& hasher)
		: HasherLayer(Forward<TInHasher>(hasher)), TAllocator(alloc)
	{
		Base::m_blocks = (TBlock*)EmptyBlock;

		Base::m_size = 0;
		Base::m_capacity = 0;
	}
	Ava_CLANG_WARNING(pop)

	Ava_FORCEINLINE ~StorageLayer()
	{
		if (iword capacity = Base::m_capacity)
		{
			TBlock* blocks = Base::m_blocks;
			iword blockCount = capacity / TBlock::Size;

			Clean(blocks, blockCount);
			Deallocate(blocks, blockCount);
		}
	}

	Ava_FORCEINLINE iword GetSize() const
	{
		return (iword)Base::m_size;
	}

	Ava_FORCEINLINE void SetSize(iword size)
	{
		Base::m_size = (u32)size;
	}

	Ava_FORCEINLINE iword GetCapacity() const
	{
		return (iword)Base::m_capacity;
	}

	Ava_FORCEINLINE TBlock* GetBlocks() const
	{
		return Base::m_blocks;
	}

	//TODO: try to improve the internal grow interface

	Ava_FORCEINLINE TBlock* SwapBlocks(iword blockCount)
	{
		TBlock* blocks = Allocate(blockCount);

		Base::m_blocks = blocks;
		Base::m_capacity = (u32)(blockCount * TBlock::Size);

		return blocks;
	}

	Ava_FORCEINLINE void FreeBlocks(TBlock* blocks, iword blockCount)
	{
		Deallocate(blocks, blockCount);
	}

	Ava_FORCEINLINE void SetDefaultBlocks()
	{
		Base::m_blocks = (TBlock*)EmptyBlock;
		Base::m_capacity = 0;
	}

private:
	Ava_FORCEINLINE TBlock* Allocate(iword blockCount)
	{
		return (TBlock*)static_cast<const TAllocator*>(this)
			->Allocate(sizeof(TBlock) * blockCount);
	}

	Ava_FORCEINLINE void Deallocate(TBlock* blocks, iword blockCount)
	{
		static_cast<const TAllocator*>(this)
			->Deallocate(blocks, sizeof(TBlock) * blockCount);
	}
};


Ava_FORCEINLINE iword GetMaxSize(iword capacity)
{
	// ~ capacity * 15/16
	return capacity - (capacity / 16);
}

Ava_FORCEINLINE uword GetIndexMask(iword capacity)
{
	return (uword)(capacity > 1 ? capacity - 1 : 1);
}

Ava_FORCEINLINE uword GetIndexShift(uword mask)
{
	return Math::Lzcnt(mask);
}

template<uword = sizeof(uword)> struct Phi;
template<> struct Phi<4> { static constexpr u32 Value = 2654435769u; };
template<> struct Phi<8> { static constexpr u64 Value = 11400714819323198485u; };

Ava_FORCEINLINE iword HashToIndex(uword hash, uword shift)
{
	return (iword)((hash * Phi<>::Value) >> shift);
}

Ava_FORCEINLINE iword Jump(iword index, Ctrl jump, uword mask)
{
	return (iword)(((uword)index + JumpOffsets[(iword)jump]) & mask);
}

template<typename T, typename TTraits, typename THasher, typename TStoragePolicy>
class Table : public StorageLayer<Block<T>, THasher, TStoragePolicy>
{
	typedef Block<T> BlockType;
	typedef StorageLayer<BlockType, THasher, TStoragePolicy> Base;

	typedef typename TTraits::template SelectKeyType<T> KeyType;

public:
	typedef typename Base::AllocatorType AllocatorType;

	typedef BasicIterator<      T> Iterator;
	typedef BasicIterator<const T> ConstIterator;

	struct InsertResult
	{
		T* Element;
		bool Inserted;
	};

	template<typename TInHasher>
	Ava_FORCEINLINE Table(const AllocatorType& alloc, TInHasher&& hasher)
		: Base(alloc, Forward<TInHasher>(hasher))
	{
	}

	template<typename TInKey>
	Ava_FORCEINLINE T* Find(const TInKey& key) const
	{
		uword hash = THasher::Hash(key);
		return FindInternal<const TInKey&>(hash, key);
	}

	template<typename TInKey>
	Ava_FORCEINLINE InsertResult Insert(const TInKey& key)
	{
		uword hash = THasher::Hash(key);
		return InsertInternal<const TInKey&>(hash, key);
	}

	template<typename TInKey>
	Ava_FORCEINLINE bool Remove(const TInKey& key)
	{
		uword hash = THasher::Hash(key);
		return RemoveInternal<const TInKey&>(hash, key);
	}

	void Reserve(iword minCapacity)
	{
		if constexpr (Base::CanGrow)
		{
			iword capacity = Base::GetCapacity();
			iword maxSize = GetMaxSize(capacity);

			if (minCapacity > maxSize)
				Rehash(Math::Max(capacity * 2, Math::Max(BlockType::Size,
					(iword)Math::RoundUpToPowerOfTwo((uword)minCapacity))));
		}
		else Ava_Assert(minCapacity < Base::GetCapacity());
	}

	void Clear(bool deallocate)
	{
		if (iword capacity = Base::GetCapacity())
		{
			BlockType* blocks = Base::GetBlocks();
			iword blockCount = capacity / BlockType::Size;

			Clean(blocks, blockCount);

			if constexpr (Base::CanGrow)
			{
				if (deallocate)
					Base::FreeBlocks(blocks, blockCount);
				Base::SetDefaultBlocks();
			}
			else Ava_UNUSED(deallocate);

			Base::SetSize(0);
		}
	}

private:
	struct Slot
	{
		Ctrl* m_ctrl;
		T* m_data;
	};

	template<typename TInKey>
	T* FindInternal(uword hash, TInKey key) const
	{
		BlockType* const blocks = Base::GetBlocks();
		const iword capacity = Base::GetCapacity();

		const uword mask = GetIndexMask(capacity);
		const uword shift = GetIndexShift(mask);

		const auto GetSlot = [&](iword index) -> Slot
		{
			iword blockIndex = index / BlockType::Size;
			iword valueIndex = index % BlockType::Size;

			BlockType* block = blocks + blockIndex;

			return Slot{
				&block->m_ctrl[valueIndex],
				&block->m_data[valueIndex]
			};
		};

		iword index = HashToIndex(hash, shift);
		Slot slot = GetSlot(index);
		Ctrl ctrl = *slot.m_ctrl;

		// No value with this hash
		if ((byte)(ctrl & Ctrl::Flag) == 0)
			return nullptr;

		ctrl = ctrl & Ctrl::Mask;
		while (true) // Search the chain
		{
			if (key == TTraits::SelectKey(*slot.m_data))
				return slot.m_data;

			if (Ava_LIKELY(ctrl == Ctrl::Last))
				return nullptr;

			index = Jump(index, ctrl, mask);
			slot = GetSlot(index);
			ctrl = *slot.m_ctrl;
		}
	}

	template<typename TInKey>
	InsertResult InsertInternal(uword hash, TInKey key)
	{
	reset:
		iword capacity = Base::GetCapacity();
		if constexpr (Base::CanGrow)
			if (Ava_UNLIKELY(capacity == 0))
			{
				Rehash(BlockType::Size);
				capacity = BlockType::Size;
			}
		BlockType* blocks = Base::GetBlocks();

		const uword mask = GetIndexMask(capacity);
		const uword shift = GetIndexShift(mask);

		const auto GetSlot = [&](iword index) -> Slot
		{
			iword blockIndex = index / BlockType::Size;
			iword valueIndex = index % BlockType::Size;

			BlockType* block = blocks + blockIndex;

			return Slot{
				&block->m_ctrl[valueIndex],
				&block->m_data[valueIndex]
			};
		};

		struct TryInsertResult
		{
			Ctrl Jump;
			Slot Slot;
		};

		const auto TryInsert = [&](iword index) -> TryInsertResult
		{
			//TODO: optimize first 16
			for (iword i = 1; i < 126; ++i)
			{
				Ctrl jump = (Ctrl)i;

				iword next = Jump(index, jump, mask);
				Slot slot = GetSlot(next);

				if (*slot.m_ctrl == Ctrl::Null)
					return { jump, slot };
			}

			Ava_CLANG_WARNING(push);
			Ava_CLANG_WARNING(ignored "-Wmissing-field-initializers");
			return { Ctrl::Null };
			Ava_CLANG_WARNING(pop);
		};

		iword index = HashToIndex(hash, shift);
		Slot slot = GetSlot(index);
		Ctrl ctrl = *slot.m_ctrl;

		if ((byte)(ctrl & Ctrl::Flag) == 0)
		{
			iword size = Base::GetSize();
			if constexpr (Base::CanGrow)
			{
				if (size == GetMaxSize(capacity))
					goto rehash;
			}
			else Ava_Assert(size < capacity);

			// if slot is part of another chain
			if (ctrl != Ctrl::Null)
			{
				iword parentIndex;
				Slot parentSlot;
				Ctrl parentCtrl;
				{
					// find parent in other chain

					uword parentHash = THasher::Hash(
						TTraits::SelectKey(*slot.m_data));
					parentIndex = HashToIndex(parentHash, shift);

					while (true)
					{
						parentSlot = GetSlot(parentIndex);
						parentCtrl = *parentSlot.m_ctrl;

						iword next = Jump(parentIndex,
							parentCtrl & Ctrl::Mask, mask);

						if (next == index)
							break;

						parentIndex = next;
					}
				}

				iword srcIndex = index;
				Slot srcSlot = slot;
				Ctrl srcCtrl = ctrl;

				while (true)
				{
					auto[jump, nextSlot] = TryInsert(parentIndex);

					// if can't insert
					if (jump == Ctrl::Null)
					{
						//TODO: make sure this is correct
						// map must be in valid state when rehashing
						// ska map uses a reserved ctrl value
						// be sure to leave an explanation

						if (srcIndex != index)
							*slot.m_ctrl = Ctrl::Null;

						goto rehash;
					}

					*parentSlot.m_ctrl = (parentCtrl & Ctrl::Flag) | jump;

					// mark next as last in chain
					*nextSlot.m_ctrl = Ctrl::Last;

					// mark source as empty
					if (srcIndex != index)
						*srcSlot.m_ctrl = Ctrl::Null;

					Relocate<T>(nextSlot.m_data, srcSlot.m_data);

					if (srcCtrl == Ctrl::Last)
						break;

					parentIndex = Jump(parentIndex, jump, mask);
					parentSlot = nextSlot;
					parentCtrl = jump;

					iword nextSrcIndex = Jump(srcIndex, srcCtrl, mask);
					Slot nextSrcSlot = GetSlot(nextSrcIndex);

					srcIndex = nextSrcIndex;
					srcSlot = nextSrcSlot;
					srcCtrl = *nextSrcSlot.m_ctrl;
				}
			}

			*slot.m_ctrl = Ctrl::Flag | Ctrl::Last;
			Base::SetSize(size + 1);

			return InsertResult{ slot.m_data, true };
		}

		while (true)
		{
			if (key == TTraits::SelectKey(*slot.m_data))
				return InsertResult{ slot.m_data, false };

			Ctrl jump = ctrl & Ctrl::Mask;
			if (Ava_LIKELY(jump == Ctrl::Last))
			{
				iword size = Base::GetSize();
				if constexpr (Base::CanGrow)
				{
					if (size == GetMaxSize(capacity))
						goto rehash;
				}
				else Ava_Assert(size < capacity);

				auto[nextJump, nextSlot] = TryInsert(index);
				if (nextJump == Ctrl::Null) goto rehash;

				*slot.m_ctrl = (ctrl & Ctrl::Flag) | nextJump;
				*nextSlot.m_ctrl = Ctrl::Last;

				Base::SetSize(size + 1);

				return InsertResult{ nextSlot.m_data, true };
			}

			index = Jump(index, jump, mask);
			slot = GetSlot(index);
			ctrl = *slot.m_ctrl;
		}

	rehash:
		if constexpr (Base::CanGrow)
		{
			Rehash(capacity * 2);
			goto reset;
		}
	}

	template<typename TInKey>
	bool RemoveInternal(uword hash, TInKey key)
	{
		BlockType* const blocks = Base::GetBlocks();
		const iword capacity = Base::GetCapacity();

		uword mask = GetIndexMask(capacity);
		uword shift = GetIndexShift(mask);

		const auto GetSlot = [&](iword index) -> Slot
		{
			iword blockIndex = index / BlockType::Size;
			iword valueIndex = index % BlockType::Size;

			BlockType* block = blocks + blockIndex;

			return Slot{
				&block->m_ctrl[valueIndex],
				&block->m_data[valueIndex]
			};
		};

		iword index = HashToIndex(hash, shift);
		Slot slot = GetSlot(index);

		// No value with this hash
		if ((byte)(*slot.m_ctrl & Ctrl::Flag) == 0)
			return false;

		Ctrl* prev = nullptr;

		while (true)
		{
			Ctrl ctrl = *slot.m_ctrl;
			Ctrl jump = ctrl & Ctrl::Mask;

			if (key == TTraits::SelectKey(*slot.m_data))
			{
				Destroy<T>(slot.m_data);
				Base::SetSize(Base::GetSize() - 1);

				if (Ava_LIKELY(jump == Ctrl::Last))
				{
					*slot.m_ctrl = Ctrl::Null;

					if (prev != nullptr)
						*prev = (*prev & Ctrl::Flag) | Ctrl::Last;
				}
				else
				{
					T* elem = slot.m_data;

					while (true)
					{
						prev = slot.m_ctrl;
						index = Jump(index, jump, mask);
						slot = GetSlot(index);

						ctrl = *slot.m_ctrl;
						if (Ava_LIKELY(ctrl == Ctrl::Last))
							break;

						jump = ctrl & Ctrl::Mask;
					}

					Relocate<T>(elem, slot.m_data);
					*prev = (*prev & Ctrl::Flag) | Ctrl::Last;
					*slot.m_ctrl = Ctrl::Null;
				}

				return true;
			}

			if (Ava_LIKELY(jump == Ctrl::Last))
				return false;

			prev = slot.m_ctrl;
			index = Jump(index, jump, mask);
			slot = GetSlot(index);
		}
	}

	Ava_FORCENOINLINE void Rehash(iword newCapacity)
	{
		Ava_Assert(newCapacity >= BlockType::Size);

		iword capacity = Base::GetCapacity();
		BlockType* oldBlocks = Base::GetBlocks();

		iword newBlockCount = newCapacity / BlockType::Size;
		BlockType* newBlocks = Base::SwapBlocks(newBlockCount);

		for (BlockType* f = newBlocks, *l = f + newBlockCount; f != l; ++f)
			for (iword index = 0; index < BlockType::Size; ++index)
				f->m_ctrl[index] = Ctrl::Null;

		if (capacity != 0)
		{
			Base::SetSize(0);

			iword oldBlockCount = capacity / BlockType::Size;
			for (BlockType* f = oldBlocks, *l = f + oldBlockCount; f != l; ++f)
				for (iword index = 0; index < BlockType::Size; ++index)
					if (f->m_ctrl[index] != Ctrl::Null)
					{
						const KeyType& key = TTraits::SelectKey(*f->m_data[index]);
						auto[slot, inserted] = InsertInternal<const KeyType&>(THasher::Hash(key), key);

						Ava_Assert(inserted);
						Relocate<T>(slot, &f->m_data[index]);
					}

			Base::FreeBlocks(oldBlocks, oldBlockCount);
		}
	}
};

} // namespace Ava::Private::Containers_HashTable
