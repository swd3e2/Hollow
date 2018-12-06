#pragma once


#ifndef __MEMORY_CHUNK_MANAGER__
#define __MEMORY_CHUNK_MANAGER__

#include "MemoryChunk.h"
#include "Allocators/PoolAllocator.h"

#include <vector>

namespace Hollow { namespace Core { namespace Memory {

	template<class T, unsigned int max_ojbects>
	class MemmoryChunkManager
	{
	private:
		std::vector<MemoryChunk *> chunks;
	public:
		MemoryChunkManager()
		{
			PoolAllocator allocator = new PoolAllocator();
		}
	};

}}}

#endif