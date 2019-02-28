#include "D3DRenderer.h"

D3DRenderer::D3DRenderer() :
	window(GetModuleHandle(NULL), SCREEN_WIDTH, SCREEN_HEIGHT)
{
	HRESULT hr = S_OK;
	RECT rc;

	GetClientRect(*window.getHWND(), &rc);
	this->width = rc.right - rc.left;
	this->height = rc.bottom - rc.top;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.OutputWindow = *window.getHWND();
		
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_1;
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags,
		featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapChainDesc, m_SwapChain.GetAddressOf(),
		&m_Device, &m_featureLevel, m_DeviceContext.GetAddressOf());

	if (hr != S_OK) {
		Hollow::Log::GetCoreLogger()->error("RenderSystem: Can't create DeviceAndSwapChain!");
	}

	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_DeviceContext->RSSetViewports(1, &vp);
	m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_RenderTarget = new D3DRenderTarget(m_Device.Get(), m_DeviceContext.Get(), m_SwapChain.Get(), width, height);
	m_DepthStencil = new D3DDepthStencil(m_Device.Get(), width, height, DXGI_FORMAT_D24_UNORM_S8_UINT, 1);

	m_BlendStateTransparancy = new D3DBlendState(m_Device.Get());

	m_SamplerStateWrap = new D3DSamplerState(m_Device.Get(), D3D11_TEXTURE_ADDRESS_WRAP);
	m_SamplerStateClamp = new D3DSamplerState(m_Device.Get(), D3D11_TEXTURE_ADDRESS_CLAMP);

	camera = new Camera();
}

D3DRenderer::~D3DRenderer()
{
	m_Device->Release();
	m_DeviceContext->Release();
	m_SwapChain->Release();
	delete	m_WVPConstantBuffer;
	delete	m_TransformConstantBuffer;
	delete	m_BlendStateTransparancy;
	delete	m_SamplerStateWrap;
	delete	m_SamplerStateClamp;
	delete	m_RenderTarget;
	delete	m_DepthStencil;
}