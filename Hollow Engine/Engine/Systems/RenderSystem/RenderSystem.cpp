#include "RenderSystem.h"

namespace Hollow {

	RenderSystem::RenderSystem(HWND * hwnd, int width, int height) :
		System()
	{
		Hollow::Log::GetCoreLogger()->debug("RenderSystem: Initialized.");
		this->hWnd = hwnd;
		this->height = height;
		this->width = width;

		this->camera = new Camera();
		this->camera->SetProjectionValues(85.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
		this->camera->SetPosition(3.0f, 2.0f, -5.0f);

		this->CreateSwapChain();
		this->InitDevices();
		this->CreateRasterizerState();
		this->CreateSamplerDesc();
		this->DefineBlending();

		constantBuffer.Init(m_Device.Get(), m_DeviceContext.Get());

		D3D11_INPUT_ELEMENT_DESC bxlayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		UINT numElements = ARRAYSIZE(bxlayout);

		vertexShader = new VertexShader(m_Device.Get(), L"Engine/Resources/Shaders/vs.hlsl", bxlayout, numElements);
		pixelShader = new PixelShader(m_Device.Get(), L"Engine/Resources/Shaders/ps.hlsl");

		std::vector<SimpleVertex> * vertices = new std::vector<SimpleVertex>;
		vertices->push_back({ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) });
		vertices->push_back({ XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) });
		vertices->push_back({ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) });
		vertices->push_back({ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) });

		vertices->push_back({ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) });
		vertices->push_back({ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) });
		vertices->push_back({ XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) });
		vertices->push_back({ XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) });

		unsigned int * indices = new unsigned int[36]{
				2, 1, 0,
				2, 0, 3,
				2, 5, 1,
				5, 7, 1,
				4, 2, 3,
				4, 5, 2,
				4, 3, 0,
				4, 0, 6,
				1, 6, 0,
				1, 7, 6,
				5, 6, 7,
				5, 4, 6
		};

		vertexBuffer = new VertexBuffer<SimpleVertex>(m_Device.Get(), vertices->data(), vertices->size());
		indexBuffer = new IndexBuffer<unsigned int>(m_Device.Get(), indices, 36);
	}

	void RenderSystem::PreUpdate(float_t dt) 
	{}

	void RenderSystem::Update(float_t dt) 
	{
		camera->Update();
		UpdateWVP();

		float ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		m_DeviceContext->ClearRenderTargetView(renderTarget->GetMainRenderTaget(), ClearColor);
		m_DeviceContext->ClearDepthStencilView(depthStencil->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_DeviceContext->OMSetRenderTargets(1, renderTarget->GetAddressOfMainRenderTaget(), depthStencil->GetDepthStencilView());
		m_DeviceContext->RSSetState(m_RasterizerState.Get());

		m_DeviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
		m_DeviceContext->IASetInputLayout(vertexShader->GetInputLayout());
		m_DeviceContext->VSSetShader(vertexShader->GetShader(), NULL, 0);
		m_DeviceContext->PSSetShader(pixelShader->GetShader(), NULL, 0);

		UINT offset = 0;
		m_DeviceContext->IASetVertexBuffers(0, 1, vertexBuffer->GetAddressOf(), vertexBuffer->StridePtr(), &offset);
		m_DeviceContext->IASetIndexBuffer(indexBuffer->Get(), DXGI_FORMAT_R32_UINT, 0);
		m_DeviceContext->DrawIndexed(36, 0, 0);
		m_pSwapChain->Present(1, 0);
	}

	void RenderSystem::PostUpdate(float_t dt) 
	{}

	void RenderSystem::InitDevices()
	{
		renderTarget = new RenderTarget(m_Device.Get(), m_DeviceContext.Get(), m_pSwapChain.Get(), width, height);
		depthStencil = new DepthStencil(m_Device.Get(), this->width, this->height, DXGI_FORMAT_D24_UNORM_S8_UINT, 1);

		D3D11_VIEWPORT vp;
		vp.Width = (float)width;
		vp.Height = (float)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		m_DeviceContext->RSSetViewports(1, &vp);
		m_DeviceContext->OMSetRenderTargets(1, renderTarget->GetAddressOfMainRenderTaget(), depthStencil->GetDepthStencilView());
		m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void RenderSystem::CreateSwapChain()
	{
		HRESULT hr = S_OK;
		RECT rc;
		GetClientRect(*hWnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		DXGI_SWAP_CHAIN_DESC sd = { 0 };
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.OutputWindow = *hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};

		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;

		hr = D3D11CreateDeviceAndSwapChain(NULL, m_driverType,
			NULL, creationFlags,
			featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd,
			m_pSwapChain.GetAddressOf(), &m_Device,
			&m_featureLevel, m_DeviceContext.GetAddressOf());

		if (hr != S_OK) {
			Hollow::Log::GetCoreLogger()->error("RenderSystem: Cant create DeviceAndSwapChain!");
		}
	}

	void RenderSystem::UpdateWVP()
	{
		constantBuffer.data.mWorld = XMMatrixTranspose(XMMatrixIdentity());
		constantBuffer.data.mView = XMMatrixTranspose(camera->GetViewMatrix());
		constantBuffer.data.mProjection = XMMatrixTranspose(camera->GetProjectionMatrix());
		constantBuffer.Update();
	}

	void RenderSystem::CreateRasterizerState()
	{
		HRESULT hr = S_OK;

		D3D11_RASTERIZER_DESC rasterizerDesc;
		rasterizerDesc.AntialiasedLineEnable = false;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
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
	}

	void RenderSystem::CreateSamplerDesc()
	{
		HRESULT hr = S_OK;

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

	void RenderSystem::DefineBlending()
	{
		HRESULT hr = S_OK;

		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));

		D3D11_RENDER_TARGET_BLEND_DESC blendingTragetDesc;
		ZeroMemory(&blendingTragetDesc, sizeof(blendingTragetDesc));

		blendingTragetDesc.BlendEnable = true;
		blendingTragetDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendingTragetDesc.DestBlend = D3D11_BLEND_BLEND_FACTOR;
		blendingTragetDesc.BlendOp = D3D11_BLEND_OP_ADD;
		blendingTragetDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		blendingTragetDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		blendingTragetDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendingTragetDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.RenderTarget[0] = blendingTragetDesc;

		hr = m_Device->CreateBlendState(&blendDesc, &Transparency);
		if (hr != S_OK) {
			Hollow::Log::GetCoreLogger()->error("RenderSystem: Cant create SampleStateClamp!");
		}
	}
}