#pragma once

#ifndef HW_D3D11_HARDWARE_BUFFER_MANAGER_H
#define HW_D3D11_HARDWARE_BUFFER_MANAGER_H

#include "Hollow/Graphics/HardwareBufferManager.h"
#include "D3D11Prerequisites.h"
#include "D3D11VertexBuffer.h"
#include "D3D11IndexBuffer.h"

namespace Hollow {
	class D3D11HardwareBufferManager : public HardwareBufferManager
	{
	public:
		virtual VertexBuffer* createVertexBuffer(Vertex* data, size_t numVertices) override;
		virtual IndexBuffer* createIndexBuffer(unsigned int* data, size_t numIndices) override;
	};
}


#endif