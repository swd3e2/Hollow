#include "HardwareBuffer.h"
#include "Hollow/Graphics/HardwareBufferManager.h"

namespace Hollow {
	s_ptr<VertexBuffer> VertexBuffer::create(const VERTEX_BUFFER_DESC& desc)
	{
		return HardwareBufferManager::instance()->create(desc);
	}

	s_ptr<IndexBuffer> IndexBuffer::create(const INDEX_BUFFER_DESC& desc)
	{
		return HardwareBufferManager::instance()->create(desc);
	}
}
