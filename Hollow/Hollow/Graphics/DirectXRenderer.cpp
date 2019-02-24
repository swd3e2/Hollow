#include "DirectXRenderer.h"

namespace Hollow {

	DirectXRenderer::DirectXRenderer(HWND* hwnd, int width, int height)
		: HollowRenderer(hwnd, width, height)
	{
		HRESULT hr = S_OK;
		RECT rc;

		GetClientRect(*hwnd, &rc);
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
		swapChainDesc.OutputWindow = *hwnd;
		
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

		D3D11_VIEWPORT vp;
		vp.Width = (float)width;
		vp.Height = (float)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		m_DeviceContext->RSSetViewports(1, &vp);
		m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		renderTarget = new RenderTarget(m_Device.Get(), m_DeviceContext.Get(), m_SwapChain.Get(), width, height);
		depthStencil = new DepthStencil(m_Device.Get(), width, height, DXGI_FORMAT_D24_UNORM_S8_UINT, 1);

		D3D11_RENDER_TARGET_BLEND_DESC blendingTragetDesc = {0};

		blendingTragetDesc.BlendEnable = true;
		blendingTragetDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendingTragetDesc.DestBlend = D3D11_BLEND_BLEND_FACTOR;
		blendingTragetDesc.BlendOp = D3D11_BLEND_OP_ADD;
		blendingTragetDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		blendingTragetDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		blendingTragetDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendingTragetDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		D3D11_BLEND_DESC blendDesc = { 0 };

		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.RenderTarget[0] = blendingTragetDesc;

		hr = m_Device->CreateBlendState(&blendDesc, &Transparency);
		if (hr != S_OK) {
			Hollow::Log::GetCoreLogger()->error("RenderSystem: Cant create SampleStateClamp!");
		}

		D3D11_RASTERIZER_DESC rasterizerDesc;
		rasterizerDesc.AntialiasedLineEnable = false;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = true;
		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.MultisampleEnable = false;
		rasterizerDesc.ScissorEnable = false;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;

		hr = m_Device->CreateRasterizerState(&rasterizerDesc, m_RasterizerState.GetAddressOf());

		if (hr != S_OK) {
			Hollow::Log::GetCoreLogger()->error("RenderSystem: Cant create RasterizerState!");
		}

		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = m_Device->CreateSamplerState(&samplerDesc, m_SamplerStateWrap.GetAddressOf());

		if (hr != S_OK) {
			Hollow::Log::GetCoreLogger()->error("RenderSystem: Cant create SamplerStateWrap!");
		}

		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		hr = m_Device->CreateSamplerState(&samplerDesc, m_SampleStateClamp.GetAddressOf());

		if (hr != S_OK) {
			Hollow::Log::GetCoreLogger()->error("RenderSystem: Cant create SampleStateClamp!");
		}
	}

	DirectXRenderer::~DirectXRenderer()
	{
		m_Device->Release();
		m_DeviceContext->Release();
		m_SwapChain->Release();
		m_RasterizerState->Release();
		m_SamplerStateWrap->Release();
		m_SampleStateClamp->Release();
		Transparency->Release();
	}
}
