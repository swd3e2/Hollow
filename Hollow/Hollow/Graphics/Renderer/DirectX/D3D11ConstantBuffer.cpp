#include "D3D11ConstantBuffer.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

D3D11ConstantBuffer::D3D11ConstantBuffer(UINT size)
{
	this->size = static_cast<UINT>(size + (16 - size % 16));

	D3D11RenderApi * r = static_cast<D3D11RenderApi*>(RenderApi::instance());
	D3D11Context& context = r->getContext();

	// Constant buffer
	D3D11_BUFFER_DESC constantBufferDesc;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.ByteWidth = this->size; // Align with 16 bytes
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	if (FAILED(context.getDevice()->CreateBuffer(&constantBufferDesc, NULL, &m_Buffer))) {
		//HW_ERROR("ConstantBuffer: can't create constant buffer.");
	}
	context.getDevice()->CreateBuffer(&constantBufferDesc, NULL, &m_Buffer);
}

D3D11ConstantBuffer::~D3D11ConstantBuffer()
{
}

bool D3D11ConstantBuffer::Update(void* data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
	D3D11Context& context = r->getContext();

	HRESULT hr = context.getDeviceContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		//HW_ERROR("ConstantBuffer: cant update buffer.");
		return false;
	}

	CopyMemory(mappedResource.pData, data, size);
	context.getDeviceContext()->Unmap(m_Buffer, 0);
	return true;
}
