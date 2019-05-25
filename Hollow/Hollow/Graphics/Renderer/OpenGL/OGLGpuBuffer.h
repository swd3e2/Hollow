#pragma once

#include "Hollow/Graphics/Renderer/Base/GPUBuffer.h"
#include "OGLPrerequisites.h"

class OGLGpuBuffer : public GPUBuffer
{
public:
	unsigned int UBO;
public:
	OGLGpuBuffer(unsigned int location, unsigned int size) :
		GPUBuffer(location, size)
	{}
	virtual void update(void* data) override
	{
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
};