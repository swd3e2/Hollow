#include "D3D11RenderTargetManager.h"
#include "D3D11Context.h"
#include "D3D11RenderApi.h"

namespace Hollow {
	RenderTarget* D3D11RenderTargetManager::create(int width, int height, RenderTargetFlags flags)
	{
		D3D11RenderTarget* renderTarget = new D3D11RenderTarget(width, height);

		HRESULT hr = S_OK;

		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11Device* device = r->getContext().getDevice();

		// Render target
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		
		if (flags == RenderTargetFlags::ACCESS_BY_CPU) {
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
		}

		textureDesc.MiscFlags = 0;
		textureDesc.CPUAccessFlags = 0;

		hr = device->CreateTexture2D(&textureDesc, NULL, &renderTarget->m_BackBuffer);
		if (hr != S_OK) {
			//HW_ERROR("RenderTarget: Cant create Texture2D!");
		}

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		hr = device->CreateRenderTargetView(renderTarget->m_BackBuffer, &renderTargetViewDesc, &renderTarget->renderTarget);
		if (hr != S_OK) {
			//HW_ERROR("RenderTarget: Cant create RenderTargetView!");
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		hr = device->CreateShaderResourceView(renderTarget->m_BackBuffer, &shaderResourceViewDesc, &renderTarget->m_ShaderResourceView);
		if (hr != S_OK) {
			//HW_ERROR("RenderTarget: Cant create ShaderResourceView!");
		}

		// Depth buffer
		DXGI_FORMAT resformat = GetDepthResourceFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
		DXGI_FORMAT srvformat = GetDepthSRVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);

		D3D11_TEXTURE2D_DESC desc = {};
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		desc.Format = resformat;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.MiscFlags = 0;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		desc.Width = width;

		hr = device->CreateTexture2D(&desc, NULL, &renderTarget->m_DepthStencilBuffer);
		if (hr != S_OK) {
			//HW_ERROR("DepthStencil: Cant create Texture2D!");
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC ddesc = {};
		ddesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		ddesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		ddesc.Texture2D.MipSlice = 0;

		hr = device->CreateDepthStencilView(renderTarget->m_DepthStencilBuffer, &ddesc, &renderTarget->m_DepthStencilView);
		if (hr != S_OK) {
			//HW_ERROR("DepthStencil: Cant create DepthStencilView!");
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = srvformat;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		srvd.Texture2D.MostDetailedMip = 0;

		hr = device->CreateShaderResourceView(renderTarget->m_DepthStencilBuffer, &srvd, &renderTarget->m_DepthResourceView);
		if (hr != S_OK) {
			//HW_ERROR("DepthStencil: Cant create ShaderResourceView!");
		}

		D3D11_DEPTH_STENCIL_DESC dssDesc;
		ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dssDesc.DepthEnable = true;
		dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dssDesc.DepthFunc = D3D11_COMPARISON_LESS;

		device->CreateDepthStencilState(&dssDesc, &renderTarget->m_DepthStencilState);

		return renderTarget;
	}

	DXGI_FORMAT D3D11RenderTargetManager::GetDepthResourceFormat(DXGI_FORMAT depthformat)
	{
		DXGI_FORMAT resformat;
		switch (depthformat)
		{
		case DXGI_FORMAT::DXGI_FORMAT_D16_UNORM:
			resformat = DXGI_FORMAT::DXGI_FORMAT_R16_TYPELESS;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
			resformat = DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
			resformat = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
			resformat = DXGI_FORMAT::DXGI_FORMAT_R32G8X24_TYPELESS;
			break;
		}

		return resformat;
	}

	DXGI_FORMAT D3D11RenderTargetManager::GetDepthSRVFormat(DXGI_FORMAT depthformat)
	{
		DXGI_FORMAT srvformat;
		switch (depthformat)
		{
		case DXGI_FORMAT::DXGI_FORMAT_D16_UNORM:
			srvformat = DXGI_FORMAT::DXGI_FORMAT_R16_FLOAT;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
			srvformat = DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
			srvformat = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
			srvformat = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
			break;
		}
		return srvformat;
	}
}