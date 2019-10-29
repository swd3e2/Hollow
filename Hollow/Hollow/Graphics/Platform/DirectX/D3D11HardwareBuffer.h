#pragma once

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/HardwareBuffer.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

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

		virtual void update(void* data, const int size) override
		{
			D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
			ID3D11Device* device = r->getContext().getDevice();
			ID3D11DeviceContext* deviceContext = r->getContext().getDeviceContext();

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			deviceContext->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			CopyMemory(mappedResource.pData, data, size);
			deviceContext->Unmap(m_Buffer, 0);
		}

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