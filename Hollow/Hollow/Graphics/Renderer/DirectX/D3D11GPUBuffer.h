#pragma once

#include "Hollow/Graphics/Renderer/Base/GPUBuffer.h"
#include "D3D11Prerequisites.h"

class D3D11GPUBuffer : public GPUBuffer
{
public:
	ID3D11Buffer* m_Buffer;
public:
	D3D11GPUBuffer(unsigned int location, unsigned int size) :
		GPUBuffer(location, size)
	{}

	virtual void update(void* data) override;
};