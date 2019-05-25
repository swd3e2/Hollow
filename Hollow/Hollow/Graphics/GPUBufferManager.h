#pragma once

#include "Hollow/Core/CModule.h"
#include "Renderer/Base/GPUBuffer.h"

class GPUBufferManager : public CModule<GPUBufferManager>
{
public:
	GPUBufferManager()
	{
		setStartedUp();
	}

	~GPUBufferManager()
	{
		setShutdown();
	}

	virtual GPUBuffer* create(unsigned int location, unsigned int size) = 0;
};