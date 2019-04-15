#ifndef HW_I_ALLOCATOR_H
#define HW_I_ALLOCATOR_H
#pragma once

/*
 * Base class for allocators
 */
class IAllocator
{
protected:
	/* Pointer to allocated memory */
	void* memory;
	/* Used memory */
	size_t used;
	/* Allocated memory */
	size_t size;
public:
	/* todo: maybe allocate memory here ? */
	IAllocator() : used(0) { }

	/*
	 * Returns memory first memory address of allocated memory
	*/
	inline void* getFirstAddress()
	{
		return memory;
	}

	/*
	 * Getter for total allocated memory size
	 */
	inline size_t getSize()
	{
		return size;
	}

	/*
	 * Getter for used memory
	 */
	inline size_t getUsed()
	{
		return used;
	}
};

#endif