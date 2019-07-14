#include "HardwareBuffer.h"
#include "Hollow/Graphics/HardwareBufferManager.h"

namespace Hollow {
	HardwareBuffer* HardwareBuffer::create(const HARDWARE_BUFFER_USAGE& desc)
	{
		return HardwareBufferManager::instance()->create(desc);
	}

}
