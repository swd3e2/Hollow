#include "D3D11RenderTarget.h"
#include "D3D11Context.h"
#include "D3D11RenderApi.h"

namespace Hollow {
	Vector4 D3D11RenderTarget::readPixel(int x, int y)
	{
		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11DeviceContext* deviceContext = r->getContext().getDeviceContext();
		ID3D11Device* device = r->getContext().getDevice();

		D3D11_BOX sourceRegion;
		sourceRegion.left = x;
		sourceRegion.right = x + 1;
		sourceRegion.top = y;
		sourceRegion.bottom = y + 1;
		sourceRegion.front = 0;
		sourceRegion.back = 1;

		ID3D11Resource* resource = nullptr;
		m_ShaderResourceView[0]->GetResource(&resource);

		D3D11_TEXTURE2D_DESC desc;
		m_BackBuffer[0]->GetDesc(&desc);
		desc.BindFlags = 0;
		desc.Width = 1;
		desc.Height = 1;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		ID3D11Texture2D* texTemp = nullptr;
		HRESULT hr = device->CreateTexture2D(&desc, NULL, &texTemp);

		deviceContext->CopySubresourceRegion(texTemp, 0, 0, 0, 0, resource, 0, &sourceRegion);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		unsigned int subresource = D3D11CalcSubresource(0, 0, 0);
		hr = deviceContext->Map(texTemp, subresource, D3D11_MAP_READ, 0, &mappedResource);
		const char* data = (const char*)mappedResource.pData;

		texTemp->Release();
		if (resource != nullptr) {
			resource->Release();
		}

		return Vector4((int)*(data + 0), (int) * (data + 4), (int) * (data + 8), (int) * (data + 12));
	}
}

