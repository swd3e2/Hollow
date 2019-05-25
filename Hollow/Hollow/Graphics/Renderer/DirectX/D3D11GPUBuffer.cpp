#include "D3D11GPUBuffer.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

void D3D11GPUBuffer::update(void* data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
	D3D11Context& context = r->getContext();

	HRESULT hr = context.getDeviceContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		//HW_ERROR("ConstantBuffer: cant update buffer.");
	}

	CopyMemory(mappedResource.pData, data, size);
	context.getDeviceContext()->Unmap(m_Buffer, 0);
}
