#include "D3D11HardwareBufferManager.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

namespace Hollow {
	VertexBuffer* D3D11HardwareBufferManager::createVertexBuffer(Vertex* data, size_t numVertices)
	{
		D3D11VertexBuffer* buffer = new D3D11VertexBuffer(data, sizeof(Vertex), numVertices);

		HRESULT hr = S_OK;

		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = sizeof(Vertex) * numVertices;
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

		hr = device->CreateBuffer(&bufferDesc, &bufferData, &buffer->buffer);

		if (hr != S_OK)
		{
			//HW_ERROR("IndexBuffer: Cant create buffer!");
		}

		return buffer;
	}

	IndexBuffer* D3D11HardwareBufferManager::createIndexBuffer(unsigned int* data, size_t numIndices)
	{
		D3D11IndexBuffer* buffer = new D3D11IndexBuffer(data, sizeof(unsigned int), numIndices);

		HRESULT hr = S_OK;

		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = sizeof(unsigned int) * numIndices;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		bufferData.pSysMem = data;
		bufferData.SysMemPitch = 0;
		bufferData.SysMemSlicePitch = 0;

		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11Device* device = r->getContext().getDevice();

		hr = device->CreateBuffer(&bufferDesc, &bufferData, &buffer->buffer);

		if (hr != S_OK)
		{
			//HW_ERROR("IndexBuffer: Cant create buffer!");
		}

		return buffer;
	}
}
