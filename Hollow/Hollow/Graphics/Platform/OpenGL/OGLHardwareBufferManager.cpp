#include "OGLHardwareBufferManager.h"

namespace Hollow {
	s_ptr<VertexBuffer> OGLHardwareBufferManager::create(const VERTEX_BUFFER_DESC& desc)
	{
		OGLVertexBuffer* buffer = new OGLVertexBuffer();
		buffer->mHardwareBuffer = std::make_shared<OGLHardwareBuffer>(desc.size, desc.stride);
		OGLHardwareBuffer* hwBuffer = std::static_pointer_cast<OGLHardwareBuffer>(buffer->mHardwareBuffer).get();

		glGenVertexArrays(1, &hwBuffer->mVao);
		glBindVertexArray(hwBuffer->mVao);

		glCreateBuffers(1, &hwBuffer->mVbo);
		glNamedBufferStorage(hwBuffer->mVbo, desc.size * desc.stride, desc.data, 0);

		glBindVertexArray(0);

		return s_ptr<VertexBuffer>(buffer);
	}

	s_ptr<IndexBuffer> OGLHardwareBufferManager::create(const INDEX_BUFFER_DESC& desc)
	{
		OGLIndexBuffer* buffer = new OGLIndexBuffer();
		buffer->mHardwareBuffer = std::make_shared<OGLHardwareBuffer>(desc.size, OGLHelper::getSize(desc.format));
		OGLHardwareBuffer* hwBuffer = std::static_pointer_cast<OGLHardwareBuffer>(buffer->mHardwareBuffer).get();
		hwBuffer->format = OGLHelper::getFormat(desc.format);

		glCreateBuffers(1, &hwBuffer->mVbo);
		glNamedBufferStorage(hwBuffer->mVbo, desc.size * OGLHelper::getSize(desc.format), desc.data, 0);

		return s_ptr<IndexBuffer>(buffer);
	}
}