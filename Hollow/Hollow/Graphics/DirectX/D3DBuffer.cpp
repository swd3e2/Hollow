#include "D3DBuffer.h"

D3DBuffer::D3DBuffer(ID3D11Device* device, void * data, UINT stride, UINT numVertices, D3D11_BIND_FLAG bindFlag)
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

	hr = device->CreateBuffer(&bufferDesc, &bufferData, &buffer);

	if (hr != S_OK)
	{
		HW_ERROR("IndexBuffer: Cant create buffer!");
	}
}
