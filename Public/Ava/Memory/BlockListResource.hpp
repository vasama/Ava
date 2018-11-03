#pragma once

#include "Ava/Debug/Assert.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Private/Ebo.hpp"
#include "Ava/Types.hpp"

namespace Ava {

namespace Private::Allocator_BlockListResource {

struct Block
{
	Block* m_next;
	uword m_offset;
};

struct Base
{
	Block* m_tail = nullptr;
	Block* m_head = nullptr;
	uword m_blockSize;
};

} // namespace Private::Allocator_BlockListResource

template<typename TAllocator>
class BlockListResource : Ava_EBO(
	Private::Allocator_BlockListResource::Base, TAllocator)
{
	typedef Ava::Private::Allocator_BlockListResource::Base Base;
	typedef Ava::Private::Allocator_BlockListResource::Block Block;

public:
	explicit Ava_FORCEINLINE BlockListResource(uword blockSize)
		: TAllocator()
	{
		Base::m_blockSize = blockSize;
	}

	explicit Ava_FORCEINLINE BlockListResource(const TAllocator& alloc, uword blockSize)
		: TAllocator(alloc)
	{
		Base::m_blockSize = blockSize;
	}

	~BlockListResource()
	{
		uword blockSize = Base::m_blockSize + sizeof(Block);
		for (Block* block = Base::m_head, *next; block; block = next)
		{
			next = block->m_next;
			TAllocator::Deallocate(block, blockSize);
		}
	}

	void* Allocate(uword size)
	{
		if (Block* tail = Base::m_tail)
		{
			uword offset = tail->m_offset;
			if (Base::m_blockSize - offset >= size)
			{
				tail->m_offset = offset + size;
				return (byte*)(tail + 1) + offset;
			}
		}

		return AllocateInternal(size);
	}

	Ava_FORCEINLINE void Deallocate(void* block, uword size)
	{
		Ava_UNUSED(block, size);
	}

	void Reset()
	{
		Block* head = Base::m_head;
		if (head) head->m_offset = 0;
		Base::m_tail = head;
	}

	Ava_FORCEINLINE const TAllocator& GetAllocator() const
	{
		return static_cast<const TAllocator&>(*this);
	}

private:
	void* AllocateInternal(uword size)
	{
		Ava_Assert(size <= Base::m_blockSize);

		Block* tail = Base::m_tail;
		Block* next;

		if (tail == nullptr || (next = tail->m_next) == nullptr)
		{
			next = (Block*)TAllocator::Allocate(Base::m_blockSize + sizeof(Block));

			if (tail) tail->m_next = next;
			else Base::m_head = next;

			next->m_next = nullptr;
		}

		next->m_offset = 0;
		Base::m_tail = next;

		return next + 1;
	}
};

} // namespace Ava
