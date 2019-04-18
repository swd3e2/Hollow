#pragma once

#ifndef HW_ARRAY_H
#define HW_ARRAY_H

#include "Hollow/Memory/PoolAllocator.h"

namespace Hollow {
	/* 
	 * Simple data-oriented container created 
	 * for storing components and entities
	 */
	template<class T, size_t size = 1024>
	class array
	{
	public:
		/* Pointer to allocator memory (probably bad idea, but who cares) */
		void* m_data;
		/* Allocator, heart of the container */
		PoolAllocator* allocator;
		/* Count of objects that container can handle without grow up */
		size_t capacity;
		/* Count of objects */
		size_t currentSize;
	public:
		array() :
			capacity(size)
		{
			allocator = new PoolAllocator(capacity, sizeof(T), alignof(T));
			m_data = allocator->getFirstAddress();
			currentSize = 0;
		}

		/*
		 * Creates object T
		 */
		template<typename ...ARGS>
		T* createObject(ARGS&& ...args)
		{
			if (currentSize + 1 > capacity) {
				this->resize(capacity * 2);
			}

			T* object = new (allocator->allocate()) T(std::forward<ARGS>(args)...);
			if (object != nullptr) {
				currentSize++;
			}

			return object;
		}

		void clear()
		{
			currentSize = 0;
			this->allocator->clear();
		}

		/*
		 * Deallocate memory that was used by object
		 */
		void destroyObject(T* object)
		{
			allocator->deallocate(object);
		}

		void resize(size_t size)
		{
			capacity = size;
			PoolAllocator* allocator = new PoolAllocator(capacity, sizeof(T), alignof(T));
			memset(allocator->getFirstAddress(), 0, this->allocator->getSize());
			memcpy(allocator->getFirstAddress(), this->allocator->getFirstAddress(), this->allocator->getSize());
			allocator->pointer = (void**)((uintptr_t)allocator->getFirstAddress() + (this->allocator->count < this->capacity ? this->capacity : this->allocator->count) * this->allocator->objectSize);
			clear();
			delete this->allocator;
			this->allocator = allocator;
			m_data = this->allocator->getFirstAddress();
		}

		size_t getSize()
		{
			return currentSize;
		}

		/*
		 * Accessing objects in array style
		 */
		T* operator[](int i)
		{
			return (T*)((uintptr_t)allocator->getFirstAddress() + (uintptr_t)i);
		}

		/*
		 * Iterator
		 */
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

			inline iterator& operator++(int)
			{
				T* temp = this;
				++p;
				return *temp;
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

		iterator begin() { return iterator((T*)this->m_data); }
		iterator end() { 
			return iterator((T*)((uintptr_t)this->m_data + this->allocator->objectSize * currentSize)); 
		}
	};
}

#endif