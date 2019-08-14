#pragma once

#ifndef HW_OGL_GPU_BUFFER_MANAGER_H
#define HW_OGL_GPU_BUFFER_MANAGER_H

#include "OGLGpuBuffer.h"
#include "OGLPrerequisites.h"
#include "Hollow/Graphics/GPUBufferManager.h"

namespace Hollow {
	class OGLGPUBufferManager : public GPUBufferManager
	{
	public:
		virtual GPUBuffer* create(unsigned int location, unsigned int size) override
		{
			unsigned int usize = static_cast<UINT>(size + (16 - size % 16)); // Align with 16 bytes
			OGLGpuBuffer* ubo = new OGLGpuBuffer(location, usize);

			glGenBuffers(1, &ubo->UBO);
			glBindBuffer(GL_UNIFORM_BUFFER, ubo->UBO);
			glBufferData(GL_UNIFORM_BUFFER, usize, NULL, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			return ubo;
		}
	};
}

#endif