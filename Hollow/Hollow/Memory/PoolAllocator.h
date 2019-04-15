#ifndef HW_POOL_ALLOCATOR_H
#define HW_POOL_ALLOCATOR_H

#pragma once
#include <memory>
#include <stdlib.h>
#include "IAllocator.h"

/*
 * Allocator that manages memory for objects with same size
 */
class PoolAllocator : public IAllocator
{
public:
	/* Pointer to next free chunk of memory */
	void** pointer;
	/* Size of object */
	unsigned int objectSize;
	/* Count of objects */
	unsigned int count;
	/* Alignment of object */
	unsigned int alignment;
public:
	PoolAllocator(unsigned int count, unsigned int objectSize, unsigned int alignment) :
		count(count), objectSize(objectSize), alignment(alignment)
	{
		this->size = count * objectSize;

		// 0x0, 0x2, 0x4, 0x6 - 2 bytes 
		// 0x0, 0x4, 0x8, 0xb - 4 bytes
		// 0x0, 0x8 - 8 bytes
		// 0x0 - 16 bytes and more
		// todo: if this will not work need to align starting memory address by myself
		this->memory = _aligned_malloc(size, alignment);

		this->clear();
		this->pointer = (void**)this->memory;
	}

	~PoolAllocator()
	{
		_aligned_free(this->memory);
	}

	/*
	 * Creates object T
	 */
	template<class T, typename ...Args>
	T* createObject()
	{
		return new (allocate()) T(std::forward(Args...));
	}

	/* 
	 * Returns memory address for next object
	 */
	void* allocate()
	{
		if (this->size < this->used + objectSize)
		{
			return nullptr;
		}

		void* mem = (void*)this->pointer;
		this->pointer = (void**)*this->pointer;
		this->used += objectSize;

		return mem;
	}

	/*
	 * Deallocates memory by given address
	 * todo: create check that memory is in valid memory range
	 * todo: maybe fill memory with 0
	 */
	void deallocate(void* mem)
	{
		void** memptr = (void**)mem;
		*memptr = (void*)this->pointer;

		this->pointer = (void**)mem;
	}

	/*
	 * Frees all memory
	 */
	void clear()
	{
		memset(this->memory, 0, size);

		void** p = (void**)this->memory;

		for (int i = 0; i < count; i++)
		{
			*p = (void*)((uintptr_t)p + objectSize);
			p = (void**)*p;
		}
	}
};

#endif 