#include "D3D11GPUBufferManager.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

GPUBuffer* D3D11BufferManager::create(unsigned int location, unsigned int size)
{
	HRESULT result = S_OK;
	unsigned int usize = static_cast<UINT>(size + (16 - size % 16));
	D3D11GPUBuffer* gpuBuffer =  new D3D11GPUBuffer(location, size);

	D3D11RenderApi * r = static_cast<D3D11RenderApi*>(RenderApi::instance());
	D3D11Context & context = r->getContext();

	// Constant buffer
	D3D11_BUFFER_DESC constantBufferDesc;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.ByteWidth = usize; // Align with 16 bytes
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	result = context.getDevice()->CreateBuffer(&constantBufferDesc, NULL, &gpuBuffer->m_Buffer);

	if (result != S_OK) {
		//HW_ERROR("ConstantBuffer: can't create constant buffer.");
	}
	return gpuBuffer;
}
