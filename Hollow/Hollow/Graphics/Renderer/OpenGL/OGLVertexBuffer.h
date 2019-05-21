#pragma once

#ifndef HW_OGL_VERTEX_BUFFER_H
#define HW_OGL_VERTEX_BUFFER_H

#include "Hollow/Graphics/Renderer/Base/VertexBuffer.h"

class OGLVertexBuffer : public VertexBuffer
{
public:
	unsigned int VAO;
	unsigned int VBO;
public:
	OGLVertexBuffer(unsigned int stride, unsigned int numIndices) :
		VertexBuffer(stride, numIndices)
	{}
};

#endif