#pragma once

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Base/HardwareBuffer.h"

namespace Hollow
{
	class D3D11HardwareBuffer : public HardwareBuffer
	{
	public:
		ID3D11Buffer* mBuffer;
	public:
		D3D11HardwareBuffer(size_t size, size_t stride) : 
			HardwareBuffer(size, stride), mBuffer(nullptr) 
		{}

		virtual ~D3D11HardwareBuffer() { SAFE_RELEASE(mBuffer); }

		ID3D11Buffer* Get() { return mBuffer; }
		ID3D11Buffer** GetAddressOf() { return &mBuffer; }
	};

	class D3D11IndexBuffer : public IndexBuffer
	{

	};

	class D3D11VertexBuffer : public VertexBuffer
	{

	};
}