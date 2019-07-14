#include "HardwareBuffer.h"
#include "Hollow/Graphics/HardwareBufferManager.h"

namespace Hollow {
	VertexBuffer* VertexBuffer::create(const VERTEX_BUFFER_DESC& desc)
	{
		return HardwareBufferManager::instance()->create(desc);
	}

	IndexBuffer* IndexBuffer::create(const INDEX_BUFFER_DESC& desc)
	{
		return HardwareBufferManager::instance()->create(desc);
	}
}
