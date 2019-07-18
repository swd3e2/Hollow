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

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)44);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (const GLvoid*)56);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)72);

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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * OGLHelper::getSize(desc.format), desc.data, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return buffer;
	}
}