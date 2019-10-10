#pragma once

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/HardwareBuffer.h"

namespace Hollow
{
	class D3D11HardwareBuffer : public HardwareBuffer
	{
	public:
		ID3D11Buffer* m_Buffer;
	public:
		D3D11HardwareBuffer(size_t size, size_t stride) : 
			HardwareBuffer(size, stride), m_Buffer(nullptr)
		{}

		virtual ~D3D11HardwareBuffer() { SAFE_RELEASE(m_Buffer); }

		ID3D11Buffer* get() const { return m_Buffer; }
		ID3D11Buffer* const * getAddressOf() const { return &m_Buffer; }
	};

	class D3D11IndexBuffer : public IndexBuffer
	{

	};

	class D3D11VertexBuffer : public VertexBuffer
	{

	};
}