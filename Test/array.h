#pragma once
#include "PoolAllocator.h"

namespace Hollow {
	template<class T, size_t size = 1024>
	class array
	{
	private:
		void* m_data;
		PoolAllocator allocator;
	public:
		array() :
			allocator(size, sizeof(T), alignof(T))
		{
			m_data = allocator.getFirstAddress();
		}

		template<typename ...ARGS>
		T* createObject(ARGS&&... args)
		{
			return new (allocator.allocate()) T(std::forward<ARGS>(args)...);
		}

		T* operator[](int i)
		{
			return (T*)((uintptr_t)allocator.getFirstAddress() + (uintptr_t)i);
		}
	};
}