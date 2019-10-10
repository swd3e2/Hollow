#pragma once

#ifndef HW_I_HARDWARE_BUFFER_MANAGER_H
#define HW_I_HARDWARE_BUFFER_MANAGER_H

#include "HardwareBuffer.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Graphics/Vertex.h"

namespace Hollow {
	class HardwareBufferManager : public CModule<HardwareBufferManager>
	{
	public:
		virtual s_ptr<VertexBuffer> create(const VERTEX_BUFFER_DESC& desc) = 0;
		virtual s_ptr<IndexBuffer> create(const INDEX_BUFFER_DESC& desc) = 0;
	};
}

#endif