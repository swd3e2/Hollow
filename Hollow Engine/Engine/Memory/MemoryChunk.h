#pragma once

#ifndef __MEMORY_CHUNK__
#define __MEMORY_CHUNK__
#include "Allocators/PoolAllocator.h"

namespace Hollow { namespace Core { namespace Memory {

	class MemoryChunk
	{
	public:
		PoolAllocator * allocator;

		MemoryChunk(PoolAllocator * in_allocator) 
		{
			allocator = in_allocator;
		}
	};

}}}

#endif