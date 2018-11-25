#pragma once
#include "../../Platform.h"
#include "IAllocator.h"

namespace ECS { namespace Memory { namespace Allocator { 

	class PoolAllocator : public IAllocator
	{
	private:
		const std::size_t	OBJECT_SIZE;
		const u8	   OBJECT_ALIGNMENT;

		void** freeList;
	public:
		PoolAllocator(std::size_t memSize, const void* mem, std::size_t objectSize, u8 objectAlignment);

		virtual ~PoolAllocator();

		virtual void* allocate(std::size_t size, u8 alignment) override;
		virtual void free(void * mem) override;
		virtual void clear() override;
	};
}}}