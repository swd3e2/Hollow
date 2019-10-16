#include "GPUBuffer.h"
#include "GPUBufferManager.h"

namespace Hollow {
	s_ptr<GPUBuffer> GPUBuffer::create(UINT32 location, UINT32 size)
	{
		return GPUBufferManager::instance()->create(location, size);
	}
}

