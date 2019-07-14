#include "D3D11HardwareBufferManager.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

namespace Hollow {
	HardwareBuffer* D3D11HardwareBufferManager::create(const HARDWARE_BUFFER_DESC& desc)
	{
		D3D11HardwareBuffer* buffer;
		D3D11_BIND_FLAG bindFlag;

		switch (desc.usage)
		{
			case HARDWARE_BUFFER_USAGE::INDEX: {
				buffer = new D3D11IndexBuffer(desc.size, desc.stride);
				bindFlag = D3D11_BIND_INDEX_BUFFER;
			} break;
			case HARDWARE_BUFFER_USAGE::VERTEX: {
				buffer = new D3D11VertexBuffer(desc.size, desc.stride);
				bindFlag = D3D11_BIND_VERTEX_BUFFER;
			} break;
		}

		if (!buffer) {
			return nullptr;
		}

		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = desc.stride * desc.size;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = bindFlag;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		bufferData.pSysMem = desc.data;
		bufferData.SysMemPitch = 0;
		bufferData.SysMemSlicePitch = 0;

		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11Device* device = r->getContext().getDevice();

		device->CreateBuffer(&bufferDesc, &bufferData, &buffer->buffer);

		return buffer;
	}
}
