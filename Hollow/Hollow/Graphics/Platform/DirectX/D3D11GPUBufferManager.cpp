#include "D3D11GPUBufferManager.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

namespace Hollow {
	GPUBuffer* D3D11GPUBufferManager::create(unsigned int location, unsigned int size)
	{
		HRESULT result = S_OK;
		unsigned int usize = static_cast<UINT>(size + (16 - size % 16)); // Align with 16 bytes
		D3D11GPUBuffer* gpuBuffer = new D3D11GPUBuffer(location, size);

		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		D3D11Context& context = r->getContext();

		// Constant buffer
		D3D11_BUFFER_DESC constantBufferDesc;
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantBufferDesc.ByteWidth = usize; 
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.StructureByteStride = 0;

		context.getDevice()->CreateBuffer(&constantBufferDesc, NULL, &gpuBuffer->m_Buffer);

		return gpuBuffer;
	}
}
