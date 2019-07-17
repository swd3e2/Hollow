#include "OGLHardwareBufferManager.h"

namespace Hollow {
	VertexBuffer* OGLHardwareBufferManager::create(const VERTEX_BUFFER_DESC& desc)
	{
		OGLVertexBuffer* buffer = new OGLVertexBuffer();
		OGLHardwareBuffer* temp = new OGLHardwareBuffer(desc.size, desc.stride);

		glCreateVertexArrays(1, &temp->mVertexArrayObject);
		glBindVertexArray(temp->mVertexArrayObject);

		glGenBuffers(1, &temp->mVertexArrayBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, temp->mVertexArrayBuffer);

		glBufferData(GL_ARRAY_BUFFER, desc.size * desc.stride, desc.data, GL_STATIC_DRAW);

		/*glCreateBuffers(1, &temp->mVertexArrayBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, temp->mVertexArrayBuffer);
		glBufferStorage(GL_ARRAY_BUFFER, desc.size * desc.stride, desc.data, GL_DYNAMIC_STORAGE_BIT);*/

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)44);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (const GLvoid*)56);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)72);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		buffer->mHardwareBuffer = temp;

		return buffer;
	}

	IndexBuffer* OGLHardwareBufferManager::create(const INDEX_BUFFER_DESC& desc)
	{
		OGLIndexBuffer* buffer = new OGLIndexBuffer();
		OGLHardwareBuffer* temp = new OGLHardwareBuffer(desc.size, desc.stride);

		glGenBuffers(1, &temp->mVertexArrayBuffer);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp->mVertexArrayBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, desc.size * desc.stride, desc.data, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		GLenum error = glGetError();

		/*glCreateBuffers(1, &temp->mVertexArrayBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp->mVertexArrayBuffer);
		glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, desc.size * desc.stride, desc.data, GL_DYNAMIC_STORAGE_BIT);*/

		buffer->mHardwareBuffer = temp;
		
		return buffer;
	}
}