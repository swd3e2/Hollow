#include "D3D11HardwareBufferManager.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

namespace Hollow {
	VertexBuffer* D3D11HardwareBufferManager::create(const VERTEX_BUFFER_DESC& desc)
	{
		D3D11VertexBuffer* buffer = new D3D11VertexBuffer();
		buffer->mHardwareBuffer = new D3D11HardwareBuffer(desc.size, desc.stride);

		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = desc.stride * desc.size;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		
		D3D11_SUBRESOURCE_DATA bufferData;
		bufferData.pSysMem = desc.data;
		bufferData.SysMemPitch = 0;
		bufferData.SysMemSlicePitch = 0;

		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11Device* device = r->getContext().getDevice();

		device->CreateBuffer(&bufferDesc, &bufferData,
			&static_cast<D3D11HardwareBuffer*>(buffer->mHardwareBuffer)->mBuffer);

		return buffer;
	}

	IndexBuffer* D3D11HardwareBufferManager::create(const INDEX_BUFFER_DESC& desc)
	{
		D3D11IndexBuffer* buffer = new D3D11IndexBuffer();
		buffer->mHardwareBuffer = new D3D11HardwareBuffer(desc.size, D3D11Helper::getSize(desc.format));

		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = D3D11Helper::getSize(desc.format) * desc.size;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		bufferData.pSysMem = desc.data;
		bufferData.SysMemPitch = 0;
		bufferData.SysMemSlicePitch = 0;

		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11Device* device = r->getContext().getDevice();

		device->CreateBuffer(&bufferDesc, &bufferData, &static_cast<D3D11HardwareBuffer*>(buffer->mHardwareBuffer)->mBuffer);

		return buffer;
	}
}
