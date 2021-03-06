#pragma once

#ifndef HW_D3D11_GPU_BUFFER_H
#define HW_D3D11_GPU_BUFFER_H

#include "Hollow/Graphics/GPUBuffer.h"
#include "D3D11Prerequisites.h"

namespace Hollow {
	class D3D11GPUBuffer : public GPUBuffer
	{
	public:
		ID3D11Buffer* m_Buffer;
	public:
		D3D11GPUBuffer(unsigned int location, unsigned int size) :
			GPUBuffer(location, size)
		{}
		
		virtual ~D3D11GPUBuffer() { SAFE_RELEASE(m_Buffer); }

		virtual void update(void* data, unsigned int size = 0) override;
	};
}

#endif