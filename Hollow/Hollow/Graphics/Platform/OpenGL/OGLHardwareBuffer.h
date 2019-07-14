#pragma once

#include "Hollow/Graphics/Base/HardwareBuffer.h"
#include "OGLPrerequisites.h"

namespace Hollow {
	class OGLHardwareBuffer : public HardwareBuffer
	{
	public:
		GLuint mVertexArrayObject;
		GLuint mVertexArrayBuffer;
		size_t vboSize;
	public:
		OGLHardwareBuffer(size_t size, size_t stride) : HardwareBuffer(size, stride) {}
	};

	class OGLIndexBuffer : public IndexBuffer
	{

	};

	class OGLVertexBuffer : public VertexBuffer
	{

	};
}