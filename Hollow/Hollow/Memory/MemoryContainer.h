#pragma once

#ifndef HW_MEMORY_CONTAINER_H
#define HW_MEMORY_CONTAINER_H

#define DEFAULT_CAPACITY 100

#include "PoolAllocator.h"
#include <vector>

namespace Hollow {
	template<typename T>
	class MemoryContainer
	{
	private:
		std::vector<PoolAllocator*> allocators;
		PoolAllocator* currentAllocator;
	public:
		MemoryContainer()
		{
			allocators.push_back(new PoolAllocator(DEFAULT_CAPACITY, sizeof(T), alignof(T)));
		}

		void* allocate()
		{
			void* allocatedMemory = nullptr;

			for (auto& it : allocators) {
				allocatedMemory = currentAllocator->allocate();
				if (allocatedMemory != nullptr) {
					break;
				}
			}

			if (allocatedMemory == nullptr) {
				PoolAllocator* allocator = new PoolAllocator(DEFAULT_CAPACITY, sizeof(T), alignof(T));
				allocatedMemory = allocator->allocate();
				allocators.push_back(allocator);
			}

			return allocatedMemory;
		}

		void* deallocate(void* memory) {
			for (auto& it : allocators) {
				if ((uintptr_t)it->getFirstAddress() > (uintptr_t)memory && (uintptr_t)memory < (uintptr_t)(it->getFirstAddress()) + (uintptr_t)it->getSize()) {
					it->deallocate(memory);
				}
			}
		}

		class iterator : public std::iterator<std::forward_iterator_tag, T>
		{
		private:
			T* p;
		public:
			iterator(const iterator& it) : p(it.p) {}
			iterator(T* p) : p(p) {}

			inline iterator& operator++()
			{
				++p;
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

		iterator begin() { return iterator(this->m_data); }
		iterator end() { return iterator(this->m_data + this->m_size); }
	};
}

#endif