#pragma once
#include "Hollow/Platform.h"
#include <cstddef>
#include <assert.h>

namespace Hollow { namespace Core { namespace Memory {

	// Returns aligned address by given alignment
	// @param address Address of memory that needs to be aligned
	// @param alignment 
	// @return void* pointer to aligned memory
	HOLLOW_API extern "C" inline void* AlignForward(const void* address, unsigned char alignment)
	{
		return (void*)((reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
	}

	// Returns alignment by given address
	// @param address Address of memory that needs to be aligned
	// @param alignment 
	// @return usngined char the size of the memory that must be aligned
	HOLLOW_API extern "C" inline unsigned char GetAdjustment(const void* address, unsigned char alignment)
	{
		unsigned char adjustment = alignment - (reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(alignment - 1));
		return adjustment == alignment ? 0 : adjustment;
	}

	HOLLOW_API extern "C" inline unsigned char GetAdjustmentWithHeader(const void* address, unsigned char alignment, unsigned char headerSize)
	{
		unsigned char adjustment = GetAdjustment(address, alignment);
		unsigned char neededSpace = headerSize;

		if (adjustment < neededSpace)
		{
			neededSpace -= adjustment;

			//Increase adjustment to fit header 
			adjustment += alignment * (neededSpace / alignment);

			if (neededSpace % alignment > 0) adjustment += alignment;
		}

		return adjustment;
	}

}}}