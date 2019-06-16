#pragma once

#ifndef HW_D3D11_VERTEX_BUFFER_H
#define HW_D3D11_VERTEX_BUFFER_H

#include "Hollow/Graphics/Renderer/Base/VertexBuffer.h"
#include "D3D11Prerequisites.h"

namespace Hollow {
	class D3D11VertexBuffer : public VertexBuffer
	{
	private:
		friend class D3D11HardwareBufferManager;
		ID3D11Buffer* buffer;
	public:
		D3D11VertexBuffer(void* data, UINT stride, size_t numVertices) :
			VertexBuffer(stride, numVertices)
		{}

		virtual ~D3D11VertexBuffer()
		{
			SAFE_RELEASE(buffer);
		}

		ID3D11Buffer* Get() { return buffer; }
		ID3D11Buffer** GetAddressOf() { return &buffer; }
	};
}

#endif