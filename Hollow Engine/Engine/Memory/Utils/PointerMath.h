#pragma once
#include "../../Platform.h"
namespace ECS { namespace Memory { namespace Allocators {

	static inline void* AlignForward(void* address, u8 alignment)
	{
		return (void*)((reinterpret_cast<uptr>(address) + static_cast<uptr>(alignment - 1)) & static_cast<uptr>(~(alignment - 1)));
	}

	static inline u8 GetAdjustment(void* address, u8 alignment)
	{
		u8 adjustment = alignment - (reinterpret_cast<uptr>(address)& static_cast<uptr>(alignment - 1));

		return adjustment == alignment ? 0 : adjustment;
	}

	static inline u8 GetAdjustment(void* address, u8 alignment, u8 extra)
	{
		u8 adjustment = GetAdjustment(address, alignment);
		u8 neededSpace = extra;

		if (adjustment < neededSpace)
		{
			neededSpace -= adjustment;

			//Increase adjustment to fit header
			adjustment += alignment * (neededSpace / alignment);

			if (neededSpace % alignment > 0)
				adjustment += alignment;
		}
		return adjustment;
	}

}}}