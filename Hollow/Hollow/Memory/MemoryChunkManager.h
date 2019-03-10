#pragma once
#include "Allocators/PoolAllocator.h"
#include "StackMemoryManager.h"
#include "Hollow/Platform.h"
#include <vector>
#include <list>
#include <assert.h>

namespace Hollow { namespace Core { namespace Memory {

	template<class T, size_t max_ojbects>
	class MemoryChunkManager : public StackMemoryManager
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
				HW_DEBUG("MemoryChunk: created, firstAddress {} {}, last address {} {}", this->begin, this->allocator->GetMemoryFirstAddress(), this->end, (void*)(this->begin + this->allocator->GetMemorySize()));
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

			inline T* operator*() const { return *m_CurrentObject; }
			inline T* operator->() const { return *m_CurrentObject; }

			inline bool operator==(iterator& other) 
			{
				return ((this->m_CurrentChunk == other.m_CurrentChunk) && (this->m_CurrentObject == other.m_CurrentObject));
			}
			inline bool operator!=(iterator& other)
			{
				return ((this->m_CurrentChunk != other.m_CurrentChunk) || (this->m_CurrentObject != other.m_CurrentObject));
			}
		};
	private:
		// Size of allocated memory for each of memory chunks
		static const size_t size = (sizeof(T) + alignof(T)) * max_ojbects;
		std::list<MemoryChunk*> chunks;
		const char* allocatorTag;
	public:
		MemoryChunkManager(const char* allocatorTag) :
			allocatorTag(allocatorTag)
		{
			HW_DEBUG("MemoryChunkManager: created, firstAddress {}", this->m_Allocator->GetMemoryFirstAddress());
			PoolAllocator* allocator = new PoolAllocator(this->allocate(size, allocatorTag), size, sizeof(T), alignof(T));
			this->chunks.push_back(new MemoryChunk(allocator));
		}

		virtual ~MemoryChunkManager()
		{
			// make sure all entities will be released!
			for (auto chunk : this->chunks)
			{
				for (auto obj : chunk->objects)
					((T*)obj)->~T();

				chunk->objects.clear();

				// free allocated allocator memory
				free((void*)chunk->allocator->GetMemoryFirstAddress());
				delete chunk->allocator;
				chunk->allocator = nullptr;

				// delete helper chunk object
				delete chunk;
				chunk = nullptr;
			}
		}

		void* CreateObject()
		{
			void* objectMemory = nullptr;

			// Iterate threw chunks, if we find place to allocate memory - we store it
			// if not - create new chunk and store in it
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

		inline iterator begin() { return iterator(this->chunks.begin(), this->chunks.end()); }
		inline iterator end() { return iterator(this->chunks.end(), this->chunks.end()); }
	};

}}}