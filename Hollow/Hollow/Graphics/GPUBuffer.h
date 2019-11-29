#pragma once

#ifndef HW_GPU_BUFFER_H
#define HW_GPU_BUFFER_H

#include "Hollow/Platform.h"

namespace Hollow {
	class GPUBuffer
	{
	private:
		unsigned int mLocation;
		unsigned int mSize;
	public:
		GPUBuffer(UINT32 location, UINT32 size) :
			mLocation(location), mSize(size)
		{}
		virtual ~GPUBuffer() {}

		static s_ptr<GPUBuffer> create(UINT32 location, UINT32 size);
		
		unsigned int getSize() const { return mSize; }
		unsigned int getLocation() const { return mLocation; }

		virtual void update(void* data, unsigned int = 0) = 0;
	};
}

#endif