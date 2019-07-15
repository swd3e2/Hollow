#include "OGLHardwareBufferManager.h"

namespace Hollow {
	VertexBuffer* OGLHardwareBufferManager::create(const VERTEX_BUFFER_DESC& desc)
	{
		OGLVertexBuffer* buffer = new OGLVertexBuffer();
		OGLHardwareBuffer* temp = new OGLHardwareBuffer(desc.size, desc.stride);

		glCreateVertexArrays(1, &temp->mVertexArrayObject);
		glCreateBuffers(1, &temp->mVertexArrayBuffer);
		glNamedBufferStorage(temp->mVertexArrayBuffer, desc.size * desc.stride, desc.data, GL_STATIC_DRAW);

		buffer->mHardwareBuffer = temp;

		return buffer;
	}

	IndexBuffer* OGLHardwareBufferManager::create(const INDEX_BUFFER_DESC& desc)
	{
		OGLIndexBuffer* buffer = new OGLIndexBuffer();
		OGLHardwareBuffer* temp = new OGLHardwareBuffer(desc.size, desc.stride);

		glCreateBuffers(1, &temp->mVertexArrayBuffer);
		glNamedBufferStorage(temp->mVertexArrayBuffer, desc.size * desc.stride, desc.data, GL_STATIC_DRAW);

		buffer->mHardwareBuffer = temp;
		
		return buffer;
	}
}