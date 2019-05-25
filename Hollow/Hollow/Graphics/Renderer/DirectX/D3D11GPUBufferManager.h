#pragma once

#include "Hollow/Graphics/GPUBufferManager.h"
#include "D3D11Prerequisites.h"
#include "D3D11GPUBuffer.h"

class D3D11BufferManager : public GPUBufferManager
{
public:
	virtual GPUBuffer* create(unsigned int location, unsigned int size) override;
};