#pragma once

#ifndef HW_GPU_BUFFER_MANAGER_H
#define HW_GPU_BUFFER_MANAGER_H

#include "Hollow/Core/CModule.h"
#include "GPUBuffer.h"
#include "Hollow/Platform.h"

namespace Hollow {
	class GPUBufferManager : public CModule<GPUBufferManager>
	{
	public:
		virtual s_ptr<GPUBuffer> create(unsigned int location, unsigned int size) = 0;
	};
}

#endif