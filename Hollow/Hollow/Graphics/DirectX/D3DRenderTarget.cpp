#include "D3DRenderTarget.h"

D3DRenderTarget::D3DRenderTarget(ID3D11Device * device, ID3D11DeviceContext * deviceContext, IDXGISwapChain * swapChain, int width, int height)
{
	HRESULT hr = S_OK;
	// Create deafult render target view
	ID3D11Texture2D * backBuffer = {};
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	hr = device->CreateRenderTargetView(backBuffer, NULL, mainRenderTarget.GetAddressOf());
	if (hr != S_OK) {
		Hollow::Log::GetCoreLogger()->error("RenderTarget: Cant create RenderTargetView!");
	}

	// Create second render target view
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	hr = device->CreateTexture2D(&textureDesc, NULL, m_BackBuffer.GetAddressOf());
	if (hr != S_OK) {
		Hollow::Log::GetCoreLogger()->error("RenderTarget: Cant create Texture2D!");
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(m_BackBuffer.Get(), &renderTargetViewDesc, secondRenderTarget.GetAddressOf());
	if (hr != S_OK) {
		Hollow::Log::GetCoreLogger()->error("RenderTarget: Cant create RenderTargetView!");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(m_BackBuffer.Get(), &shaderResourceViewDesc, m_ShaderResourceView.GetAddressOf());
	if (hr != S_OK) {
		Hollow::Log::GetCoreLogger()->error("RenderTarget: Cant create RenderTargetView!");
	}
}
