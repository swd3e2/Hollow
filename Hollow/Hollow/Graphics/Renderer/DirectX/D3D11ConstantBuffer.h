#pragma once
#include <d3d11.h>
#include "Hollow/Platform.h"
#include "D3D11RenderApi.h"

class D3D11ConstantBuffer
{
private:
	D3D11ConstantBuffer(const D3D11ConstantBuffer &rhs);
private:
	ID3D11Buffer* m_Buffer;
	D3D11Context* context;
	UINT size;
public:
	/*
	 * @param UINT size size of structure 
	 */
	D3D11ConstantBuffer(UINT size) {
		this->size = static_cast<UINT>(size + (16 - size % 16));

		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(IRenderApi::instance());
		this->context = r->getContext();
		// Constant buffer
		D3D11_BUFFER_DESC constantBufferDesc;
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.ByteWidth = this->size; // Align with 16 bytes
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.StructureByteStride = 0;

		if (FAILED(context->device->CreateBuffer(&constantBufferDesc, NULL, &m_Buffer))) {
			HW_ERROR("ConstantBuffer: can't create constant buffer.");
		}
		context->device->CreateBuffer(&constantBufferDesc, NULL, &m_Buffer);
	}

	~D3D11ConstantBuffer()
	{
	}

	ID3D11Buffer * const* GetAddressOf() const
	{
		return &m_Buffer;
	}

	bool Update(void* data)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = context->deviceContext->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			HW_ERROR("ConstantBuffer: cant update buffer.");
			return false;
		}
		CopyMemory(mappedResource.pData, data, size);
		context->deviceContext->Unmap(m_Buffer, 0);
		return true;
	}
};
