#pragma once

#ifndef __POINTER_MATH_UTILS__
#define __POINTER_MATH_UTILS__
#include <cstddef>
#include <assert.h>

namespace Hollow { namespace Core { namespace Memory {

	// Returns aligned address by given alignment
	// @param address Address of memory that needs to be aligned
	// @param alignment 
	// @return void* pointer to aligned memory
	static inline void* AlignForward(const void* address, unsigned char alignment)
	{
		return (void*)((reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
	}

	// Returns alignment by given address
	// @param address Address of memory that needs to be aligned
	// @param alignment 
	// @return usngined char the size of the memory that must be aligned
	static inline unsigned char GetAdjustment(const void* address, unsigned char alignment)
	{
		unsigned char adjustment = alignment - (reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(alignment - 1));
		return adjustment == alignment ? 0 : adjustment;
	}

}}}

#endif