#pragma once

#ifndef HW_OGL_HARDWARE_BUFFER_MANAGER_H
#define HW_OGL_HARDWARE_BUFFER_MANAGER_H

#include "Hollow/Graphics/HardwareBufferManager.h"
#include "OGLPrerequisites.h"
#include "OGLHardwareBuffer.h"
#include "OGLHelper.h"

namespace Hollow {
	class OGLHardwareBufferManager : public HardwareBufferManager
	{
	public:
		virtual VertexBuffer* create(const VERTEX_BUFFER_DESC& desc) override;
		virtual IndexBuffer* create(const INDEX_BUFFER_DESC& desc) override;
	};
}

#endif