#pragma once
#include "Hollow/Memory/PoolAllocator.h"

namespace Hollow {
	template<class T, size_t size = 1024>
	class array
	{
	public:
		void* m_data;
		PoolAllocator* allocator;
		size_t capacity;
		size_t currentSize;
	public:
		array() :
			capacity(size)
		{
			allocator = new PoolAllocator(capacity, sizeof(T), alignof(T));
			m_data = allocator->getFirstAddress();
			currentSize = 0;
		}

		template<typename ...ARGS>
		T* createObject(ARGS&&... args)
		{
			if (currentSize + 1 > capacity) {
				capacity = capacity * 2;
				PoolAllocator* allocator = new PoolAllocator(capacity, sizeof(T), alignof(T));
				memset(allocator->getFirstAddress(), 0, this->allocator->size);
				memcpy(allocator->getFirstAddress(), this->allocator->getFirstAddress(), this->allocator->size);
				allocator->pointer = (void**)((uintptr_t)allocator->getFirstAddress() + this->allocator->count * this->allocator->objectSize);
				delete this->allocator;
				this->allocator = allocator;
				m_data = this->allocator->getFirstAddress();
			}

			T* object = new (allocator->allocate()) T(std::forward<ARGS>(args)...);
			if (object != nullptr) {
				currentSize++;
			}

			return object;
		}

		T* operator[](int i)
		{
			return (T*)((uintptr_t)allocator->getFirstAddress() + (uintptr_t)i);
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

		iterator begin() { return iterator((T*)this->m_data); }
		iterator end() { return iterator((T*)((uintptr_t)this->m_data + this->capacity)); }
	};
}