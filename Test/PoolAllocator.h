#pragma once
#include <memory>
#include <stdlib.h>

inline void* AlignForward(const void* address, size_t alignment)
{
	// + alignment - 1 для того, если при побитовом и адрес уменьшится не было segfault
	return (void*)((reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
}

class PoolAllocator
{
public:
	void* firstAddress;
	void** pointer;
	unsigned int size;
	unsigned int objectSize;
	unsigned int usedSpace;
	unsigned int count;
	unsigned int alignment;
public:
	PoolAllocator(unsigned int count, unsigned int objectSize, unsigned int alignment) :
		count(count), objectSize(objectSize), alignment(alignment)
	{
		this->size = count * objectSize;
		this->usedSpace = 0;
		// 0x0, 0x2, 0x4, 0x6 - 2 bytes 
		// 0x0, 0x4, 0x8, 0xb - 4 bytes
		// 0x0, 0x8 - 8 bytes
		// 0x0 - 16 bytes and more
		this->firstAddress = _aligned_malloc(size, alignment);

		this->clear();
		this->pointer = (void**)this->firstAddress;
	}

	~PoolAllocator()
	{
		_aligned_free(this->firstAddress);
	}

	template<class T, typename ...Args>
	T* createObject()
	{
		return new (allocate()) T(std::forward(Args...));
	}

	void* allocate()
	{
		if (this->size < this->usedSpace + objectSize)
		{
			return nullptr;
		}

		void* mem = (void*)this->pointer;
		this->pointer = (void**)*this->pointer;
		this->usedSpace += objectSize;

		return mem;
	}

	void deallocate(void* mem)
	{
		void** memptr = (void**)mem;
		*memptr = (void*)this->pointer;

		this->pointer = (void**)mem;
	}

	void clear()
	{
		memset(this->firstAddress, 0, size);

		void** p = (void**)this->firstAddress;

		for (int i = 0; i < count; i++)
		{
			*p = (void*)((uintptr_t)p + objectSize);
			p = (void**)*p;
		}
	}

	inline void* getFirstAddress()
	{
		return this->firstAddress;
	}
};
