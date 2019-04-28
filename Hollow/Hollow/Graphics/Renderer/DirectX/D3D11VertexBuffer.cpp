#include "D3D11VertexBuffer.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

D3D11VertexBuffer::D3D11VertexBuffer(void* data, UINT stride, UINT numVertices) :
	VertexBuffer(stride, numVertices)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.ByteWidth = stride * numVertices;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA bufferData;
	bufferData.pSysMem = data;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
	ID3D11Device* device = r->getContext().getDevice();

	hr = device->CreateBuffer(&bufferDesc, &bufferData, &buffer);

	if (hr != S_OK)
	{
		//HW_ERROR("IndexBuffer: Cant create buffer!");
	}
}

D3D11VertexBuffer::~D3D11VertexBuffer()
{
	SAFE_RELEASE(buffer);
}
