#pragma once

#include "Hollow/Graphics/HardwareBuffer.h"
#include "OGLPrerequisites.h"
#include "OGLInputLayout.h"

namespace Hollow {
	class OGLHardwareBuffer : public HardwareBuffer
	{
	private:
		friend class OGLHardwareBufferManager;
	public:
		GLuint mVao;
		GLuint mVbo;
		size_t vboSize;
		GLuint format;
	public:
		OGLHardwareBuffer(size_t size, size_t stride) : HardwareBuffer(size, stride) {}

		virtual void update(void* data, const int size) override
		{
			glNamedBufferSubData(mVbo, 0, size, data);
		}

		virtual ~OGLHardwareBuffer()
		{
			glDeleteBuffers(1, &mVbo);
			glDeleteVertexArrays(1, &mVao);
		}
	};

	class OGLIndexBuffer : public IndexBuffer
	{

	};

	class OGLVertexBuffer : public VertexBuffer
	{

	};
}