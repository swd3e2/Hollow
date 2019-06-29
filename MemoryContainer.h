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
	protected:
		class MemoryChunk
		{
			PoolAllocator* allocator;
			std::list<T*> objects;

			uintptr_t start;
			uintptr_t end;

			MemoryChunk(PoolAllocator* allocator) :
				allocator(allocator)
			{
				start = (uintptr_t)allocator->getFirstAddress();
				end = start + (uintptr_t)allocator->getSize();
			}
		};

		std::vector<MemoryChunk*> chunks;
	public:
		MemoryContainer()
		{
			chunks.push_back(new MemoryChunk(new PoolAllocator(DEFAULT_CAPACITY, sizeof(T), alignof(T))));
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
				it->objects.push_back((T*)allocatedMemory);
				chunks.push_back(chunk);
			}

			return allocatedMemory;
		}

		void* deallocate(void* memory) {
			for (auto& it : chunks) {
				if ((uintptr_t)it->allocator->getFirstAddress() > (uintptr_t)memory && (uintptr_t)memory < (uintptr_t)(it->allocator->getFirstAddress()) + (uintptr_t)it->allocator->getSize()) {
					it->objects.remove((T*)allocatedMemory);
					it->deallocate(memory);
				}
			}
		}

		class iterator : public std::iterator<std::forward_iterator_tag, T>
		{
		private:
			std::vector<MemoryChunk*>::iterator begin;
			std::vector<MemoryChunk*>::iterator end;

			std::list<T*>::iterator current;
		public:
			iterator(const iterator& it) : p(it.p) {}
			iterator(MemoryChunk* begin, MemoryChunk* end)
				: begin(begin), end(end)
			{
				if (begin != end) {
					current = (*begin)->objects.begin();
				} else {
					current = (*std::prev(end))->objects.end();
				}
			}

			inline iterator& operator++()
			{
				p++;
				return *this;
			}

			inline iterator& operator--()
			{	
				--p;
				return *this;
			}

			inline bool operator==(iterator& other) const { return p == other.p; };
			inline bool operator!=(iterator& other) const { return p != other.p; };
			inline T& operator*() const { return *p; }
			inline T* operator->() const { return *p; }
		};

		iterator begin() { return iterator(chunks.begin(), chunks.end()); }
		iterator end() { return iterator(chunks.end(), chunks.end()); }
	};
}

#endif