#pragma once

#ifndef HW_OGL_INDEX_BUFFER_H
#define HW_OGL_INDEX_BUFFER_H

#include "Hollow/Graphics/Renderer/Base/IndexBuffer.h"

class OGLIndexBuffer : public IndexBuffer
{
public:
	unsigned int VAO;
	unsigned int VBO;
public:
	OGLIndexBuffer(unsigned int stride, unsigned int numIndices) :
		IndexBuffer(stride, numIndices)
	{}
};

#endif