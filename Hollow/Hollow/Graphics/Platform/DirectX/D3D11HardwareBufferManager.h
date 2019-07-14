#pragma once

#ifndef HW_D3D11_HARDWARE_BUFFER_MANAGER_H
#define HW_D3D11_HARDWARE_BUFFER_MANAGER_H

#include "Hollow/Graphics/HardwareBufferManager.h"
#include "D3D11Prerequisites.h"
#include "D3D11HardwareBuffer.h"

namespace Hollow {
	class D3D11HardwareBufferManager : public HardwareBufferManager
	{
	public:
		virtual VertexBuffer* create(const VERTEX_BUFFER_DESC& desc) override;
		virtual IndexBuffer* create(const INDEX_BUFFER_DESC& desc) override;
	};
}


#endif