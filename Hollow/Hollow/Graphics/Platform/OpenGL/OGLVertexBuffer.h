#pragma once

#ifndef HW_OGL_VERTEX_BUFFER_H
#define HW_OGL_VERTEX_BUFFER_H

#include "Hollow/Graphics/VertexBuffer.h"
#include "OGLPrerequisites.h"

namespace Hollow {
	class OGLVertexBuffer : public VertexBuffer
	{
	public:
		unsigned int VAO;
		unsigned int VBO;
	public:
		OGLVertexBuffer(unsigned int stride, unsigned int numIndices) :
			VertexBuffer(stride, numIndices)
		{}
		virtual ~OGLVertexBuffer()
		{
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
		}
	};
}

#endif