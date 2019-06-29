#pragma once

#ifndef HW_I_HARDWARE_BUFFER_MANAGER_H
#define HW_I_HARDWARE_BUFFER_MANAGER_H

#include "Renderer/Base/VertexBuffer.h"
#include "Renderer/Base/IndexBuffer.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Graphics/Vertex.h"

namespace Hollow {
	class HardwareBufferManager : public CModule<HardwareBufferManager>
	{
	public:
		virtual VertexBuffer* createVertexBuffer(Vertex* data, size_t numVertices) = 0;
		virtual IndexBuffer* createIndexBuffer(unsigned int* data, size_t numIndices) = 0;
	};
}

#endif