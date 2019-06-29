#pragma once

#ifndef HW_ALLOCATOR_UTILS_H
#define HW_ALLOCATOR_UTILS_H
#include <stdlib.h>

namespace Hollow {
	// Returns first address that fits given aligment
	// more on https://www.gamedev.net/articles/programming/general-and-gameplay-programming/c-custom-memory-allocation-r3010/
	inline void* AlignForward(const void* address, size_t alignment)
	{
		// + alignment - 1 для того, если при побитовом и адрес уменьшится не было segfault
		return (void*)((reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
	}
}

#endif