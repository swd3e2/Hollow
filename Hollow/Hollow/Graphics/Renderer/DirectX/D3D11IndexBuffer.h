#pragma once

#ifndef HW_D3D11_INDEX_BUFFER_H
#define HW_D3D11_INDEX_BUFFER_H


#include "Hollow/Graphics/Renderer/Base/IIndexBuffer.h"
#include "D3D11RenderApi.h"
#include <d3d11.h>

class D3D11IndexBuffer : public IIndexBuffer
{
private:
	ID3D11Buffer* buffer;
	UINT stride;
	UINT bufferSize = 0;
public:
	D3D11IndexBuffer(void* data, UINT stride, UINT numVertices, D3D11_BIND_FLAG bindFlag)
	{
		HRESULT hr = S_OK;

		bufferSize = numVertices;
		this->stride = stride;
		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = stride * numVertices;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = bindFlag; // D3D11_BIND_VERTEX_BUFFER; D3D11_BIND_INDEX_BUFFER
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		bufferData.pSysMem = data;
		bufferData.SysMemPitch = 0;
		bufferData.SysMemSlicePitch = 0;

		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(IRenderApi::instance());
		ID3D11Device* device = r->getContext()->device;

		hr = device->CreateBuffer(&bufferDesc, &bufferData, &buffer);

		if (hr != S_OK)
		{
			HW_ERROR("IndexBuffer: Cant create buffer!");
		}
	}

	~D3D11IndexBuffer()
	{
		SAFE_RELEASE(buffer);
	}

	ID3D11Buffer* Get() { return buffer; }
	ID3D11Buffer** GetAddressOf() { return &buffer; }
	size_t BufferSize() const { return bufferSize; }
	const UINT Stride() const { return stride; }
	const UINT* StridePtr() const { return &stride; }
};

#endif