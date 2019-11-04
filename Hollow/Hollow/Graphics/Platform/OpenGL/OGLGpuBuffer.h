#pragma once

#ifndef HW_OGL_GPU_BUFFER_H
#define HW_OGL_GPU_BUFFER_H

#include "Hollow/Graphics/GPUBuffer.h"
#include "OGLPrerequisites.h"

namespace Hollow {
	class OGLGpuBuffer : public GPUBuffer
	{
	public:
		unsigned int UBO;
	public:
		OGLGpuBuffer(unsigned int location, unsigned int size) :
			GPUBuffer(location, size)
		{}

		virtual void update(void* data, unsigned int size = 0) override
		{
			glBindBuffer(GL_UNIFORM_BUFFER, UBO);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, size == 0 ? this->getSize() : size, data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	};
}

#endif