#pragma once

#ifndef HW_D3D_VERTEX_BUFFER_H
#define HW_D3D_VERTEX_BUFFER_H

#include "Hollow/Graphics/Renderer/Base/IVertexBuffer.h"
#include "D3DRenderApi.h"
#include <d3d11.h>

class D3DVertexBuffer : public IVertexBuffer
{
private:
	ID3D11Buffer* buffer;
	UINT stride;
	UINT bufferSize = 0;
public:
	D3DVertexBuffer(void* data, UINT stride, UINT numVertices, D3D11_BIND_FLAG bindFlag)
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

		D3D11RenderAPI* rs = static_cast<D3D11RenderAPI*>(D3DRenderApi::instance());
		D3D11Device& device = rs->getPrimaryDevice();

		hr = device->CreateBuffer(&bufferDesc, &bufferData, &buffer);

		if (hr != S_OK)
		{
			HW_ERROR("IndexBuffer: Cant create buffer!");
		}
	}

	~D3DVertexBuffer()
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