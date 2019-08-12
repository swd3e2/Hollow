#pragma once

#ifndef HW_MEMORY_CONTAINER_H
#define HW_MEMORY_CONTAINER_H

#define DEFAULT_CAPACITY 1000

#include "PoolAllocator.h"

#include <vector>
#include <list>

namespace Hollow {
	template<typename T>
	class MemoryContainer
	{
	public:
		class MemoryChunk
		{
		public:
			PoolAllocator* allocator;
			std::list<T*> objects;

			uintptr_t start;
			uintptr_t end;
		public:
			MemoryChunk(PoolAllocator* allocator) :
				allocator(allocator)
			{
				start = (uintptr_t)allocator->getFirstAddress();
				end = start + (uintptr_t)allocator->getSize();
			}
		};

		class iterator : public std::iterator<std::forward_iterator_tag, T>
		{
		private:
			typename std::list<MemoryChunk*>::iterator currentChunk;
			typename std::list<MemoryChunk*>::iterator end;

			typename std::list<T*>::iterator currentObject;
		public:
			iterator(typename std::list<MemoryChunk*>::iterator begin, typename std::list<MemoryChunk*>::iterator end)
				: currentChunk(begin), end(end)
			{
				if (begin != end) {
					currentObject = (*currentChunk)->objects.begin();
				} else {
					currentObject = (*std::prev(end))->objects.end();
				}
			}

			inline iterator& operator++()
			{
				currentObject++;
				if (currentObject == (*currentChunk)->objects.end()) {
					currentChunk++;
					if (currentChunk != end) {
						currentObject = (*currentChunk)->objects.begin();
					}
				}
				return *this;
			}

			inline bool operator==(const iterator& other) const { 
				return currentChunk == other.currentChunk && currentObject == other.currentObject;
			};

			inline bool operator!=(const iterator& other) const { 
				return currentChunk != other.currentChunk && currentObject != other.currentObject;
			};

			inline T& operator*() const { return *(*currentObject); }
			inline T* operator->() const { return *currentObject; }
		};
	public:
		std::list<MemoryChunk*> chunks;
	public:
		MemoryContainer()
		{
			chunks.push_back(new MemoryChunk(new PoolAllocator(DEFAULT_CAPACITY, sizeof(T), alignof(T))));
		}

		~MemoryContainer()
		{
			for (auto it : chunks) {
				for (T*& object : it->objects) {
					object->~T();
					it->allocator->deallocate(object);
				}
				delete it;
			}
			chunks.clear();
		}

		void* allocate()
		{
			void* allocatedMemory = nullptr;

			for (auto& it : chunks) {
				allocatedMemory = it->allocator->allocate();
				if (allocatedMemory != nullptr) {
					it->objects.push_back((T*)allocatedMemory);
					break;
				}
			}

			if (allocatedMemory == nullptr) {
				MemoryChunk* chunk = new MemoryChunk(new PoolAllocator(DEFAULT_CAPACITY, sizeof(T), alignof(T)));
				allocatedMemory = chunk->allocator->allocate();
				chunk->objects.push_back((T*)allocatedMemory);
				chunks.push_back(chunk);
			}

			return allocatedMemory;
		}

		void deallocate(void* memory) {
			for (auto& it : chunks) {
				if (it->start <= (uintptr_t)memory && (uintptr_t)memory < it->end) {
					((T*)memory)->~T();
					it->objects.remove((T*)memory);
					it->allocator->deallocate(memory);
				}
			}
		}

		iterator begin() { return iterator(chunks.begin(), chunks.end()); }
		iterator end() { return iterator(chunks.end(), chunks.end()); }
	};
}

#endif