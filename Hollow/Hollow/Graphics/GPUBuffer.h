#pragma once

#ifndef HW_GPU_BUFFER_H
#define HW_GPU_BUFFER_H

#include "Hollow/Platform.h"

namespace Hollow {
	class GPUBuffer
	{
	protected:
		unsigned int location;
		unsigned int size;
	public:
		GPUBuffer(UINT32 location, UINT32 size) :
			location(location), size(size)
		{}

		static s_ptr<GPUBuffer> create(UINT32 location, UINT32 size);
		
		unsigned int getSize() const { return size; }
		unsigned int getLocation() const { return location; }

		virtual void update(void* data) = 0;
	};
}

#endif