#include "OGLHardwareBufferManager.h"

namespace Hollow {
	VertexBuffer* OGLHardwareBufferManager::create(const VERTEX_BUFFER_DESC& desc)
	{
		OGLVertexBuffer* buffer = new OGLVertexBuffer();
		buffer->mHardwareBuffer = new OGLHardwareBuffer(desc.size, desc.stride);

		glGenVertexArrays(1, &static_cast<OGLHardwareBuffer*>(buffer->mHardwareBuffer)->mVao);
		glBindVertexArray(static_cast<OGLHardwareBuffer*>(buffer->mHardwareBuffer)->mVao);

		glGenBuffers(1, &static_cast<OGLHardwareBuffer*>(buffer->mHardwareBuffer)->mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, static_cast<OGLHardwareBuffer*>(buffer->mHardwareBuffer)->mVbo);

		glBufferData(GL_ARRAY_BUFFER, desc.size * desc.stride, desc.data, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return buffer;
	}

	IndexBuffer* OGLHardwareBufferManager::create(const INDEX_BUFFER_DESC& desc)
	{
		OGLIndexBuffer* buffer = new OGLIndexBuffer();
		buffer->mHardwareBuffer = new OGLHardwareBuffer(desc.size, OGLHelper::getSize(desc.format));
		OGLHardwareBuffer* hwBuffer = static_cast<OGLHardwareBuffer*>(buffer->mHardwareBuffer);
		hwBuffer->format = OGLHelper::getFormat(desc.format);

		glGenBuffers(1, &hwBuffer->mVbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hwBuffer->mVbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, desc.size * OGLHelper::getSize(desc.format), desc.data, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return buffer;
	}
}