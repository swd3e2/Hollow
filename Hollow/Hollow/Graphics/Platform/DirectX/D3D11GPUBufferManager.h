#pragma once

#ifndef HW_D3D11_BUFFER_MANAGER_H
#define HW_D3D11_BUFFER_MANAGER_H

#include "Hollow/Graphics/GPUBufferManager.h"
#include "D3D11Prerequisites.h"
#include "D3D11GPUBuffer.h"

namespace Hollow {
	class D3D11GPUBufferManager : public GPUBufferManager
	{
	public:
		virtual s_ptr<GPUBuffer> create(unsigned int location, unsigned int size) override;
	};
}

#endif