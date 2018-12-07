#pragma once


#ifndef __MEMORY_CHUNK_MANAGER__
#define __MEMORY_CHUNK_MANAGER__

#include "MemoryChunk.h"
#include "Allocators/PoolAllocator.h"
#include "StackMemoryManager.h"

#include <vector>
#include <list>
#include <assert.h>

namespace Hollow { namespace Core { namespace Memory {

	template<class T, unsigned int max_ojbects>
	class MemoryChunkManager : public StackMemoryManager
	{
	private:
		// Size of allocated memory for each of memory chunks
		static const size_t size = (sizeof(T) + alignof(T)) * max_ojbects;
		std::vector<MemoryChunk<T> *> chunks;
		const char* allocatorTag;
	public:
		MemoryChunkManager(const char* allocatorTag) :
			allocatorTag(allocatorTag)
		{
			PoolAllocator* allocator = new PoolAllocator(this->allocate(size, allocatorTag), size, sizeof(T), alignof(T));
			this->chunks.push_back(new MemoryChunk<T>(allocator));
		}

		void* CreateObject()
		{
			void* objectMemory = nullptr;

			// Iterate threw chunks, if we find place to allocate memory - we store it
			// if not - create new chunk and store here
			for (auto chunk : this->chunks)
			{
				if (chunk->objects.size() > max_ojbects)
					continue;

				objectMemory = chunk->allocator->allocate(sizeof(T), alignof(T));

				if (objectMemory != nullptr)
				{
					chunk->objects.push_back((T*)objectMemory);
					break;
				}
			}

			if (objectMemory == nullptr)
			{
				PoolAllocator* allocator = new PoolAllocator(this->allocate(size, allocatorTag), size, sizeof(T), alignof(T));
				MemoryChunk<T>* memoryChunk = new MemoryChunk<T>(allocator);

				this->chunks.push_back(memoryChunk);
				objectMemory = memoryChunk->allocator->allocate(sizeof(T), alignof(T));
				memoryChunk->objects.push_back((T*)objectMemory);
				assert(objectMemory != nullptr && "Looks like we out of memory");
			}

			return objectMemory;
		}

		void DestroyObject(void* mem)
		{
			unsigned int memory = (unsigned int)mem;
			for (auto chunk : this->chunks)
			{
				if (chunk->begin <= mem && mem > chunk->end)
				{
					chunk->allocator->free(mem);
					chunk->objects.remove((T*)mem);
					return;
				}
			}
		}
	};

}}}

#endif;