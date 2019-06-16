#pragma once

#ifndef HW_D3D11_INDEX_BUFFER_H
#define HW_D3D11_INDEX_BUFFER_H

#include "Hollow/Graphics/Renderer/Base/IndexBuffer.h"
#include "D3D11Prerequisites.h"

namespace Hollow {
	class D3D11IndexBuffer : public IndexBuffer
	{
	private:
		friend class D3D11HardwareBufferManager;
		ID3D11Buffer* buffer;
	public:
		D3D11IndexBuffer(void* data, UINT stride, UINT numIndices) :
			IndexBuffer(stride, numIndices)
		{}

		virtual ~D3D11IndexBuffer()
		{
			SAFE_RELEASE(buffer);
		}

		ID3D11Buffer* Get() { return buffer; }
		ID3D11Buffer** GetAddressOf() { return &buffer; }
	};
}

#endif