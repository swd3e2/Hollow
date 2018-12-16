#ifndef __MEMORY_CHUNK_MANAGER_H__
#define __MEMORY_CHUNK_MANAGER_H__

#include "Allocators/PoolAllocator.h"
#include "StackMemoryManager.h"
#include <vector>
#include <list>
#include <assert.h>
#include "Platform.h"

namespace Hollow { namespace Core { namespace Memory {

	template<class T, size_t max_ojbects>
	class HOLLOW_API MemoryChunkManager : public StackMemoryManager
	{
	public:
		class MemoryChunk
		{
		public:
			PoolAllocator * allocator;
			size_t begin;
			size_t end;
			std::list<T*> objects;

			MemoryChunk(PoolAllocator * in_allocator)
			{
				allocator = in_allocator;
				this->begin = (size_t)this->allocator->GetMemoryFirstAddress();
				this->end = this->begin + this->allocator->GetMemorySize();
				this->objects.clear();
				Hollow::Log::GetCoreLogger()->debug("MemoryChunk: created, firstAddress {} {}, last address {} {}", this->begin, this->allocator->GetMemoryFirstAddress(), this->end, (void*)(this->begin + this->allocator->GetMemorySize()));
			}
		};

		class iterator : public std::iterator<std::forward_iterator_tag, T>
		{
			typename std::list<MemoryChunk*>::iterator m_CurrentChunk;
			typename std::list<MemoryChunk*>::iterator m_End;
			typename std::list<T*>::iterator m_CurrentObject;
		public:
			iterator(typename std::list<MemoryChunk*>::iterator begin, typename std::list<MemoryChunk*>::iterator end) :
				m_CurrentChunk(begin),
				m_End(end)
			{
				if (begin != end)
					m_CurrentObject = (*m_CurrentChunk)->objects.begin();
				else
					m_CurrentObject = (*std::prev(m_End))->objects.end();
			}

			inline iterator& operator++()
			{
				// move to next object in current chunk
				m_CurrentObject++;

				// if we reached end of list, move to next chunk
				if (m_CurrentObject == (*m_CurrentChunk)->objects.end())
				{
					m_CurrentChunk++;

					if (m_CurrentChunk != m_End)
					{
						// set object iterator to begin of next chunk list
						assert((*m_CurrentChunk) != nullptr);
						m_CurrentObject = (*m_CurrentChunk)->objects.begin();
					}
				}

				return *this;
			}

			inline T& operator*() const { return *m_CurrentObject; }
			inline T* operator->() const { return *m_CurrentObject; }

			inline bool operator==(iterator& other) {
				return ((this->m_CurrentChunk == other.m_CurrentChunk) && (this->m_CurrentObject == other.m_CurrentObject));
			}
			inline bool operator!=(iterator& other)
			{
				return ((this->m_CurrentChunk != other.m_CurrentChunk) && (this->m_CurrentObject != other.m_CurrentObject));
			}
		};
	private:
		// Size of allocated memory for each of memory chunks
		static const size_t size = (sizeof(T) + alignof(T)) * max_ojbects;
		std::vector<MemoryChunk*> chunks;
		const char* allocatorTag;
	public:
		MemoryChunkManager(const char* allocatorTag) :
			allocatorTag(allocatorTag)
		{
			Hollow::Log::GetCoreLogger()->debug("MemoryChunkManager: created, firstAddress {}", this->m_Allocator->GetMemoryFirstAddress());

			PoolAllocator* allocator = new PoolAllocator(this->allocate(size, allocatorTag), size, sizeof(T), alignof(T));
			this->chunks.push_back(new MemoryChunk(allocator));
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
				MemoryChunk* memoryChunk = new MemoryChunk(allocator);

				this->chunks.push_back(memoryChunk);
				objectMemory = memoryChunk->allocator->allocate(sizeof(T), alignof(T));
				memoryChunk->objects.push_back((T*)objectMemory);
				assert(objectMemory != nullptr && "Looks like we out of memory");
			}

			return objectMemory;
		}

		void DestroyObject(void* mem)
		{
			size_t memory = (size_t)mem;
			for (auto chunk : this->chunks)
			{
				if (memory >= chunk->begin && memory < chunk->end)
				{
					chunk->allocator->free(mem);
					chunk->objects.remove((T*)mem);
					return;
				}
			}
		}

		inline size_t GetMemoryUsed() { return this->m_Allocator->GetMemoryUsed(); }
	};

}}}

#endif;