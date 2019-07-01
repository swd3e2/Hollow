#include "D3D11RenderTargetManager.h"
#include "D3D11Context.h"
#include "D3D11RenderApi.h"

namespace Hollow {
	RenderTarget* D3D11RenderTargetManager::create(int width, int height, RENDER_TARGET_DESC desc)
	{
		D3D11RenderTarget* renderTarget = new D3D11RenderTarget(width, height, desc.count);
		
		renderTarget->m_BackBuffer = new ID3D11Texture2D*[desc.count];
		renderTarget->renderTarget = new ID3D11RenderTargetView*[desc.count];
		renderTarget->m_ShaderResourceView = new ID3D11ShaderResourceView*[desc.count];

		HRESULT hr = S_OK;

		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11Device* device = r->getContext().getDevice();

		DXGI_FORMAT textureFormat = GetTextureFormat(desc.textureFormat);

		// Render target
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = textureFormat;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		textureDesc.MiscFlags = 0;
		textureDesc.CPUAccessFlags = 0;

		for (int i = 0; i < desc.count; i++) {
			device->CreateTexture2D(&textureDesc, NULL, &renderTarget->m_BackBuffer[i]);
		}

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureFormat;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		for (int i = 0; i < desc.count; i++) {
			hr = device->CreateRenderTargetView(renderTarget->m_BackBuffer[i], &renderTargetViewDesc, &renderTarget->renderTarget[i]);
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = textureFormat;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		for (int i = 0; i < desc.count; i++) {
			device->CreateShaderResourceView(renderTarget->m_BackBuffer[i], &shaderResourceViewDesc, &renderTarget->m_ShaderResourceView[i]);
		}

		// Depth buffer
		DXGI_FORMAT resformat = GetDepthResourceFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);
		DXGI_FORMAT srvformat = GetDepthSRVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);

		D3D11_TEXTURE2D_DESC depthTextureDesc = {};
		depthTextureDesc.ArraySize = 1;
		depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		depthTextureDesc.Format = resformat;
		depthTextureDesc.Height = height;
		depthTextureDesc.MipLevels = 1;
		depthTextureDesc.MiscFlags = 0;
		depthTextureDesc.SampleDesc.Count = 1;
		depthTextureDesc.SampleDesc.Quality = 0;
		depthTextureDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		depthTextureDesc.Width = width;

		hr = device->CreateTexture2D(&depthTextureDesc, NULL, &renderTarget->m_DepthStencilBuffer);
		

		D3D11_DEPTH_STENCIL_VIEW_DESC ddesc = {};
		ddesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		ddesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		ddesc.Texture2D.MipSlice = 0;

		hr = device->CreateDepthStencilView(renderTarget->m_DepthStencilBuffer, &ddesc, &renderTarget->m_DepthStencilView);
		

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = srvformat;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		srvd.Texture2D.MostDetailedMip = 0;

		hr = device->CreateShaderResourceView(renderTarget->m_DepthStencilBuffer, &srvd, &renderTarget->m_DepthResourceView);
		
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

	DXGI_FORMAT D3D11RenderTargetManager::GetTextureFormat(RENDER_TARGET_TEXTURE_FORMAT format)
	{
		switch (format)
		{
		case R8G8B8A8: {
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		} break;
		case R32G32B32A32: {
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		} break;
		default:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		}
	}
}