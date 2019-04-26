#pragma once

#ifndef HW_D3D11_HARDWARE_BUFFER_MANAGER_H
#define HW_D3D11_HARDWARE_BUFFER_MANAGER_H

#include "Hollow/Graphics/Renderer/IHardwareBufferManager.h"
#include "D3D11VertexBuffer.h"
#include "D3D11IndexBuffer.h"

class D3D11HardwareBufferManager : public IHardwareBufferManager
{
public:
	virtual IVertexBuffer* createVertexBuffer() override;
	virtual IIndexBuffer* createIndexBuffer() override;
};

#endif