#pragma once

#ifndef HW_OGL_INDEX_BUFFER_H
#define HW_OGL_INDEX_BUFFER_H

#include "Hollow/Graphics/Renderer/Base/IndexBuffer.h"

namespace Hollow {
	class OGLIndexBuffer : public IndexBuffer
	{
	public:
		unsigned int IBO;
	public:
		OGLIndexBuffer(unsigned int stride, unsigned int numIndices) :
			IndexBuffer(stride, numIndices)
		{}
		virtual ~OGLIndexBuffer()
		{
			glDeleteBuffers(1, &IBO);
		}
	};
}

#endif