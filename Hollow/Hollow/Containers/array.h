#pragma once

#ifndef HW_ARRAY_H
#define HW_ARRAY_H

#include "Hollow/Memory/PoolAllocator.h"

namespace Hollow {
	/* 
	 * Simple data-oriented container created 
	 * for storing components and entities
	 */
	template<class T>
	class array
	{
	public:
		/* Pointer to allocator memory (probably bad idea, but who cares) */
		void* m_data;
		/* Allocator, heart of the container */
		PoolAllocator* allocator;
		/* Count of objects that container can handle without grow up */
		size_t m_Capacity;
		/* Count of objects */
		size_t m_Size;
	public:
		array(size_t size) :
			m_Capacity(size)
		{
			allocator = new PoolAllocator(m_Capacity, sizeof(T), alignof(T));
			m_data = allocator->getFirstAddress();
			m_Size = 0;
		}

		~array()
		{
			delete allocator;
		}

		/*
		 * Creates object T
		 */
		template<typename ...ARGS>
		T* createObject(ARGS&& ...args)
		{
			if (m_Size + 1 > m_Capacity) {
				this->resize(m_Capacity * 2);
			}
			T* object = new (allocator->allocate()) T(std::forward<ARGS>(args)...);
			if (object != nullptr) {
				m_Size++;
			}

			return object;
		}

		void clear()
		{
			m_Size = 0;
			this->allocator->clear();
		}

		/*
		 * Deallocate memory that was used by object
		 */
		void destroyObject(T* object)
		{
			allocator->deallocate(object);
		}

	   /*
		* Return capacity of container
		*/
		void resize(size_t size)
		{
			m_Capacity = size;
			PoolAllocator* allocator = new PoolAllocator(m_Capacity, sizeof(T), alignof(T));
			memset(allocator->getFirstAddress(), 0, this->allocator->getSize());
			memcpy(allocator->getFirstAddress(), this->allocator->getFirstAddress(), this->allocator->getSize());
			allocator->pointer = (void**)((uintptr_t)allocator->getFirstAddress() + (this->allocator->count < m_Capacity ? m_Capacity : this->allocator->count) * this->allocator->objectSize);
			clear();
			delete this->allocator;
			this->allocator = allocator;
			m_data = this->allocator->getFirstAddress();
		}

	   /*
	    * Return capacity of container
	    */
		size_t capacity() const
		{
			return m_Capacity;
		}

	   /*
		* Return size of container
		*/
		size_t size() const
		{
			return m_Size;
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
			return iterator((T*)((uintptr_t)this->m_data + this->allocator->objectSize * m_Size));
		}
	};
}

#endif