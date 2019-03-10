#include "D3DRenderTarget.h"

D3DRenderTarget::D3DRenderTarget(ID3D11Device * device, ID3D11DeviceContext * deviceContext, IDXGISwapChain * swapChain, int width, int height)
{
	HRESULT hr = S_OK;
	// Create deafult render target view
	ID3D11Texture2D * backBuffer = {};
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	hr = device->CreateRenderTargetView(backBuffer, NULL, &mainRenderTarget);
	if (hr != S_OK) {
		HW_ERROR("RenderTarget: Cant create RenderTargetView!");
	}
}

D3DRenderTarget::~D3DRenderTarget()
{
}
