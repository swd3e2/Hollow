#include "D3DRenderTarget.h"

D3DRenderTarget::D3DRenderTarget(int width, int height, RenderTargetType type, DXGI_FORMAT format, IDXGISwapChain * swapChain)
{
	HRESULT hr = S_OK;
	// Create deafult render target view
	ID3D11Texture2D * backBuffer = {};

	D3DContext context = D3DRenderer::instance()->getContext();

	if (type == RenderTargetType::MAIN)
	{
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

		hr = context.device->CreateRenderTargetView(backBuffer, NULL, &renderTarget);
		if (hr != S_OK) {
			HW_ERROR("RenderTarget: Cant create RenderTargetView!");
		}
	} else if (type == RenderTargetType::SECONDARY) {
		// Create second render target view
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		hr = context.device->CreateTexture2D(&textureDesc, NULL, &m_BackBuffer);
		if (hr != S_OK) {
			HW_ERROR("RenderTarget: Cant create Texture2D!");
		}

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		hr = context.device->CreateRenderTargetView(m_BackBuffer, &renderTargetViewDesc, &renderTarget);
		if (hr != S_OK) {
			HW_ERROR("RenderTarget: Cant create RenderTargetView!");
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		hr = context.device->CreateShaderResourceView(m_BackBuffer, &shaderResourceViewDesc, &m_ShaderResourceView);
		if (hr != S_OK) {
			HW_ERROR("RenderTarget: Cant create ShaderResourceView!");
		}
	}
}

D3DRenderTarget::~D3DRenderTarget()
{

}
