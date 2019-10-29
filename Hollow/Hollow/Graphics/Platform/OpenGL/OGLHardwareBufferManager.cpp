#include "OGLHardwareBufferManager.h"

namespace Hollow {
	s_ptr<VertexBuffer> OGLHardwareBufferManager::create(const VERTEX_BUFFER_DESC& desc)
	{
		OGLVertexBuffer* buffer = new OGLVertexBuffer();
		buffer->mHardwareBuffer = new OGLHardwareBuffer(desc.size, desc.stride);
		OGLHardwareBuffer* hwBuffer = static_cast<OGLHardwareBuffer*>(buffer->mHardwareBuffer);

		glGenVertexArrays(1, &static_cast<OGLHardwareBuffer*>(buffer->mHardwareBuffer)->mVao);
		glBindVertexArray(static_cast<OGLHardwareBuffer*>(buffer->mHardwareBuffer)->mVao);

		glCreateBuffers(1, &hwBuffer->mVbo);
		glNamedBufferStorage(hwBuffer->mVbo, desc.size * desc.stride, desc.data, 0);

		glBindVertexArray(0);

		return s_ptr<VertexBuffer>(buffer);
	}

	s_ptr<IndexBuffer> OGLHardwareBufferManager::create(const INDEX_BUFFER_DESC& desc)
	{
		OGLIndexBuffer* buffer = new OGLIndexBuffer();
		buffer->mHardwareBuffer = new OGLHardwareBuffer(desc.size, OGLHelper::getSize(desc.format));
		OGLHardwareBuffer* hwBuffer = static_cast<OGLHardwareBuffer*>(buffer->mHardwareBuffer);
		hwBuffer->format = OGLHelper::getFormat(desc.format);

		glCreateBuffers(1, &hwBuffer->mVbo);
		glNamedBufferStorage(hwBuffer->mVbo, desc.size * OGLHelper::getSize(desc.format), desc.data, 0);

		return s_ptr<IndexBuffer>(buffer);
	}
}