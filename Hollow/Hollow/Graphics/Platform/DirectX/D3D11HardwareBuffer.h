#pragma once

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Base/HardwareBuffer.h"

namespace Hollow
{
	class D3D11HardwareBuffer : public HardwareBuffer
	{
	protected:
		ID3D11Buffer* buffer;
	public:
		D3D11HardwareBuffer(size_t size, size_t stride) : HardwareBuffer(size, stride) {}
		virtual ~D3D11HardwareBuffer() { SAFE_RELEASE(buffer); }

		ID3D11Buffer* Get() { return buffer; }
		ID3D11Buffer** GetAddressOf() { return &buffer; }
	};

	class D3D11IndexBuffer : public D3D11HardwareBuffer
	{
	public:
		D3D11IndexBuffer(size_t size, size_t stride) : D3D11HardwareBuffer(size, stride) {}
	};

	class D3D11VertexBuffer : public D3D11HardwareBuffer
	{
	public:
		D3D11VertexBuffer(size_t size, size_t stride) : D3D11HardwareBuffer(size, stride) {}
	};
}