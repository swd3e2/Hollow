#pragma once
#include "Allocators/PoolAllocator.h"
#include <list>
#include "GlobalMemoryUser.h"
#include <cstddef>
#include "../Platform.h"

namespace ECS { namespace Memory {

	template<class OBJECT_CLASS, std::size_t MAX_CHUNK_OBJECTS>
	class MemoryChunkAllocator : public GlobalMemoryUser
	{
	private:
		static const std::size_t MAX_OBJECTS = MAX_CHUNK_OBJECTS;
		static const std::size_t perChunkMem = (sizeof(OBJECT_CLASS) + alignof(OBJECT_CLASS)) * MAX_OBJECTS;

		const char* m_AllocatorTag;

		// Memory chunk allocator class
		class MemoryChunk 
		{
		public: 
			Allocators::PoolAllocator* allocator;
			std::list<OBJECT_CLASS> objects;

			MemoryChunk(Allocators::PoolAllocator * in_allocator) : allocator(in_allocator) 
			{
				this->objects.clear();
			}
		};

	protected:
		std::list<MemoryChunk*> memoryChunks;
	public:
		MemoryChunkAllocator(const char* allocatorTag = nullptr) :
			m_AllocatorTag(allocatorTag)
		{
			Allocators::PoolAllocator * allocator = new Allocators::PoolAllocator(perChunkMem, Allocate(perChunkMem, allocatorTag), sizeof(OBJECT_CLASS), alignof(OBJECT_CLASS));
			this->memoryChunks.push_back(new MemoryChunk(allocator));
		}

		virtual ~MemoryChunkAllocator()
		{
			for (auto chunk : this->memoryChunks)
			{
				for (auto obj : chunk->objects)
					((OBJECT_CLASS*)obj)->~OBJECT_CLASS();
				
				chunk->objects.clear();

				Free((void*)chunk)->allocator->GetMemoryAddress0();
				delete chunk->allocator();

				delete chunk;
				chunk = nullptr;
			}
		}

		/// Creating object
		void* CreateObject()
		{
			void* slot = nullptr;

			for (auto chunk : this->memoryChunks)
			{
				if (chunk->objects.size() > MAX_OBJECTS)
					continue;

				slot = chunk->allocate(sizeof(OBJECT_CLASS), alignof(OBJECT_CLASS));
				if (slot != nullptr)
				{
					chunk->objects.push_back((OBJECT_CLASS*)slot);
					break;
				}
			}

			if (slot == nullptr)
			{
				Allocators::PoolAllocator * allocator = new Allocators::PoolAllocator(perChunkMem, Allocate(perChunkMem, this->m_AllocatorTag), sizeof(OBJECT_CLASS), alignof(OBJECT_CLASS));
				MemoryChunk * memChunk = new MemoryChunk(allocator);

				this->memoryChunks.push_back(memChunk);

				slot = memChunk->allocator->allocate(sizeof(OBJECT_CLASS), alignof(OBJECT_CLASS));

				assert(slot != nullptr && "Unable to create new object. Out of memory?!");
				memChunk->objects.clear();
				memChunk->objects.push_back((OBJECT_CLASS*)slot);
			}
			return slot;
		}
	};

}}