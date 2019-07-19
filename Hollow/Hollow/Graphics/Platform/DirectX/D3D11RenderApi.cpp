#include "D3D11RenderApi.h"
#include "D3D11Context.h"
#include "D3D11RenderTarget.h"
#include "D3D11PixelShader.h"
#include "D3D11VertexShader.h"
#include "D3D11ConstantBuffer.h"
#include "D3D11BlendState.h"
#include "D3D11SamplerState.h"
#include "D3D11RasterizerState.h"
#include "D3D11Context.h"
#include "D3D11ConstBufferMapping.h"
#include "D3D11RenderTarget.h"
#include "D3D11TextureManager.h"
#include "D3D11HardwareBufferManager.h"
#include "D3D11ShaderManager.h"
#include "D3D11Shader.h"
#include "D3D11GPUBufferManager.h"
#include "D3D11RenderTargetManager.h"
#include "D3D11InputLayout.h"
#include "D3D11Helper.h"
#include "Hollow/Common/Helper.h"
#include "D3D11PipelineStateManager.h"
#include "D3D11InputLayoutManager.h"

namespace Hollow {
	D3D11RenderApi::D3D11RenderApi(int width, int height)
		: width(width), height(height)
	{
		vp.Width = (float)width;
		vp.Height = (float)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
	}

	D3D11RenderApi::~D3D11RenderApi()
	{
		HardwareBufferManager::shutdown();
		TextureManager::shutdown();
		ShaderManager::shutdown();
		GPUBufferManager::shutdown();
		RenderTargetManager::shutdown();
		PipelineStateManager::shutdown();
		InputLayoutManager::shutdown();
	}

	void D3D11RenderApi::onStartUp()
	{
		HRESULT hr = S_OK;
		RECT rc;

		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		IDXGISwapChain* swapChain;

		DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.OutputWindow = *(static_cast<D3D11Win32Window*>(Window::instance())->getHWND());

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};

		D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_1;
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;

		hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags,
			featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
			&device, &m_featureLevel, &deviceContext);

		context = new D3D11Context(device, deviceContext, swapChain);

		// Render main target
		ID3D11Texture2D* backBuffer = {};
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& backBuffer);

		hr = device->CreateRenderTargetView(backBuffer, NULL, &renderTarget);

		//Depth stencil
		D3D11_TEXTURE2D_DESC desc;
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.MiscFlags = 0;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		desc.Width = width;

		hr = device->CreateTexture2D(&desc, NULL, &m_DepthStencilBuffer);

		D3D11_DEPTH_STENCIL_VIEW_DESC ddesc = {};
		ddesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		ddesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		ddesc.Texture2D.MipSlice = 0;

		hr = device->CreateDepthStencilView(m_DepthStencilBuffer, &ddesc, &m_DepthStencilView);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		srvd.Texture2D.MostDetailedMip = 0;

		hr = device->CreateShaderResourceView(m_DepthStencilBuffer, &srvd, &m_DepthResourceView);

		deviceContext->RSSetViewports(1, &vp);

		deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Texture samplers
		m_SamplerStateClamp = new D3D11SamplerState(D3D11_TEXTURE_ADDRESS_CLAMP);
		m_SamplerStateWrap = new D3D11SamplerState(D3D11_TEXTURE_ADDRESS_WRAP);
		deviceContext->PSSetSamplers(0, 1, m_SamplerStateClamp->GetSamplerState());
		deviceContext->DSSetSamplers(1, 1, m_SamplerStateClamp->GetSamplerState());
		deviceContext->DSSetSamplers(0, 1, m_SamplerStateWrap->GetSamplerState());
		deviceContext->PSSetSamplers(1, 1, m_SamplerStateWrap->GetSamplerState());

		// Rasterizers
		m_cullNone = new D3D11RasterizerState(D3D11_CULL_MODE::D3D11_CULL_NONE, D3D11_FILL_MODE::D3D11_FILL_SOLID);
		m_cullFront = new D3D11RasterizerState(D3D11_CULL_MODE::D3D11_CULL_FRONT, D3D11_FILL_MODE::D3D11_FILL_SOLID);
		m_cullBack = new D3D11RasterizerState(D3D11_CULL_MODE::D3D11_CULL_BACK, D3D11_FILL_MODE::D3D11_FILL_SOLID);

		setRasterizerState(m_cullNone);

		blendState = new D3D11BlendState();

		D3D11_DEPTH_STENCIL_DESC dssDesc;
		ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dssDesc.DepthEnable = true;
		dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

		{
			dssDesc.DepthFunc = D3D11_COMPARISON_NEVER;
			hr = device->CreateDepthStencilState(&dssDesc, &m_DepthStencilStateNever);
		}
		{
			dssDesc.DepthFunc = D3D11_COMPARISON_LESS;
			hr = device->CreateDepthStencilState(&dssDesc, &m_DepthStencilStateLess);
		}
		{
			dssDesc.DepthFunc = D3D11_COMPARISON_EQUAL;
			hr = device->CreateDepthStencilState(&dssDesc, &m_DepthStencilStateEqual);
		}
		{
			dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			hr = device->CreateDepthStencilState(&dssDesc, &m_DepthStencilStateLequal);
		}
		{
			dssDesc.DepthFunc = D3D11_COMPARISON_GREATER;
			hr = device->CreateDepthStencilState(&dssDesc, &m_DepthStencilStateGreater);
		}
		{
			dssDesc.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;
			hr = device->CreateDepthStencilState(&dssDesc, &m_DepthStencilStateNotEqual);
		}
		{
			dssDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
			hr = device->CreateDepthStencilState(&dssDesc, &m_DepthStencilStateAlways);
		}

		HardwareBufferManager::startUp<D3D11HardwareBufferManager>();
		TextureManager::startUp<D3D11TextureManager>();
		ShaderManager::startUp<D3D11ShaderManager>();
		GPUBufferManager::startUp<D3D11GPUBufferManager>();
		RenderTargetManager::startUp<D3D11RenderTargetManager>();
		PipelineStateManager::startUp<D3D11PipelineStateManager>();
		InputLayoutManager::startUp<D3D11InputLayoutManager>();
	}

	void D3D11RenderApi::setTexture(UINT slot, Texture* texture)
	{
		D3D11Texture* d3dTexture = static_cast<D3D11Texture*>(texture);
		if (texture != nullptr && d3dTexture->m_TextureShaderResource != nullptr) {
			context->getDeviceContext()->PSSetShaderResources(slot, 1, &d3dTexture->m_TextureShaderResource);
			context->getDeviceContext()->DSSetShaderResources(slot, 1, &d3dTexture->m_TextureShaderResource);
			context->getDeviceContext()->CSGetUnorderedAccessViews(0, 1, &d3dTexture->m_UnorderedAccessView);
		} else {
			context->getDeviceContext()->PSSetShaderResources(slot, 1, pSRV);
			context->getDeviceContext()->DSSetShaderResources(slot, 1, pSRV);
			context->getDeviceContext()->CSGetUnorderedAccessViews(slot, uavs, pUAV);
		}
	}

	void D3D11RenderApi::unsetTexture(UINT slot)
	{
		context->getDeviceContext()->PSSetShaderResources(slot, 1, pSRV);
		context->getDeviceContext()->DSSetShaderResources(slot, 1, pSRV);
	}

	void D3D11RenderApi::setTextureColorBuffer(UINT slot, RenderTarget* renderTarget, UINT targetNum)
	{
		D3D11RenderTarget* d3dRenderTarget = static_cast<D3D11RenderTarget*>(renderTarget);
		context->getDeviceContext()->PSSetShaderResources(slot, 1, &d3dRenderTarget->GetShaderResourceView()[targetNum]);
	}

	void D3D11RenderApi::setTextureDepthBuffer(UINT slot, RenderTarget* renderTarget)
	{
		D3D11RenderTarget* d3dRenderTarget = static_cast<D3D11RenderTarget*>(renderTarget);
		auto ptr = d3dRenderTarget->GetDepthStencilResource();
		context->getDeviceContext()->PSSetShaderResources(slot, 1, &ptr);
	}

	void D3D11RenderApi::setIndexBuffer(IndexBuffer* buffer)
	{
		D3D11IndexBuffer* buff = (D3D11IndexBuffer*)(buffer);
		context->getDeviceContext()->IASetIndexBuffer(static_cast<D3D11HardwareBuffer*>(buff->mHardwareBuffer)->Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void D3D11RenderApi::setVertexBuffer(VertexBuffer* buffer)
	{
		D3D11VertexBuffer* buff = (D3D11VertexBuffer*)(buffer);
		context->getDeviceContext()->IASetVertexBuffers(0, 1, static_cast<D3D11HardwareBuffer*>(buff->mHardwareBuffer)->GetAddressOf(), static_cast<D3D11HardwareBuffer*>(buff->mHardwareBuffer)->getStridePtr(), &this->offset);
	}

	void D3D11RenderApi::clearRenderTarget(RenderTarget* renderTarget, const float* color)
	{
		if (renderTarget != nullptr) {
			D3D11RenderTarget* d3d11RenderTarget = static_cast<D3D11RenderTarget*>(renderTarget);
			for (int i = 0; i < d3d11RenderTarget->count; i++) {
				context->getDeviceContext()->ClearRenderTargetView(d3d11RenderTarget->GetRenderTaget()[i], color);
			}
			context->getDeviceContext()->ClearDepthStencilView(d3d11RenderTarget->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		} else {
			context->getDeviceContext()->ClearRenderTargetView(this->renderTarget, color);
			context->getDeviceContext()->ClearDepthStencilView(this->m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}

	void D3D11RenderApi::setDepthTestFunction(DEPTH_TEST_FUNCTION func)
	{
		switch (func) {
			case DEPTH_TEST_FUNCTION::NEVER:
			{
				context->getDeviceContext()->OMSetDepthStencilState(this->m_DepthStencilStateNever, 0);
			} break;
			case DEPTH_TEST_FUNCTION::LESS:
			{
				context->getDeviceContext()->OMSetDepthStencilState(this->m_DepthStencilStateLess, 0);
			} break;
			case DEPTH_TEST_FUNCTION::EQUAL:
			{
				context->getDeviceContext()->OMSetDepthStencilState(this->m_DepthStencilStateEqual, 0);
			} break;
			case DEPTH_TEST_FUNCTION::LEQUAL:
			{
				context->getDeviceContext()->OMSetDepthStencilState(this->m_DepthStencilStateLequal, 0);
			} break;
			case DEPTH_TEST_FUNCTION::GREATER:
			{
				context->getDeviceContext()->OMSetDepthStencilState(this->m_DepthStencilStateGreater, 0);
			} break;
			case DEPTH_TEST_FUNCTION::NOT_EQUAL:
			{
				context->getDeviceContext()->OMSetDepthStencilState(this->m_DepthStencilStateNotEqual, 0);
			} break;
			case DEPTH_TEST_FUNCTION::ALWAYS:
			{
				context->getDeviceContext()->OMSetDepthStencilState(this->m_DepthStencilStateAlways, 0);
			} break;
		}
	}

	void D3D11RenderApi::setCullMode(CULL_MODE mode)
	{
		switch (mode)
		{
		case Hollow::CULL_NONE:
			setRasterizerState(m_cullNone);
			break;
		case Hollow::CULL_FRONT:
			setRasterizerState(m_cullFront);
			break;
		case Hollow::CULL_BACK:
			setRasterizerState(m_cullBack);
			break;
		}
	}

	void D3D11RenderApi::setPipelineState(PipelineState* pipeline)
	{
		D3D11PipelineState* d3dPipeline = static_cast<D3D11PipelineState*>(pipeline);

		D3D11VertexShader* d3dVertexShader = d3dPipeline->getVertexShader();
		if (d3dVertexShader != nullptr) {
			context->getDeviceContext()->VSSetShader(d3dVertexShader->GetShader(), NULL, 0);
		}

		D3D11PixelShader* d3dPixelShader = d3dPipeline->getPixelShader();
		if (d3dPixelShader != nullptr) {
			context->getDeviceContext()->PSSetShader(d3dPixelShader->GetShader(), NULL, 0);
		}

		D3D11GeometryShader* d3dGeometryShader = d3dPipeline->getGeometryShader();
		if (d3dGeometryShader != nullptr) {
			context->getDeviceContext()->GSSetShader(d3dGeometryShader->GetShader(), NULL, 0);
		}

		D3D11DomainShader* d3dDomainShader = d3dPipeline->getDomainShader();
		if (d3dDomainShader != nullptr) {
			context->getDeviceContext()->DSSetShader(d3dDomainShader->GetShader(), NULL, 0);
		}

		D3D11HullShader* d3dHullShader = d3dPipeline->getHullShader();
		if (d3dHullShader != nullptr) {
			context->getDeviceContext()->HSSetShader(d3dHullShader->GetShader(), NULL, 0);
		}

		D3D11ComputeShader* d3dComputeShader = d3dPipeline->getComputeShader();
		if (d3dComputeShader != nullptr) {
			context->getDeviceContext()->CSSetShader(d3dComputeShader->GetShader(), NULL, 0);
		}
	}

	void D3D11RenderApi::setSampler(int slot, D3D11SamplerState* sampler)
	{
		context->getDeviceContext()->PSSetSamplers(slot, 1, sampler->GetSamplerState());
		context->getDeviceContext()->DSSetSamplers(slot, 1, sampler->GetSamplerState());
	}

	void D3D11RenderApi::setRenderTarget(RenderTarget* renderTarget)
	{
		if (renderTarget != nullptr) {
			D3D11RenderTarget* d3d11RenderTarget = static_cast<D3D11RenderTarget*>(renderTarget);
			context->getDeviceContext()->OMSetRenderTargets(d3d11RenderTarget->count, d3d11RenderTarget->GetRenderTaget(), d3d11RenderTarget->GetDepthStencilView());
		} else {
			context->getDeviceContext()->OMSetRenderTargets(3, nullRTV, NULL);
			context->getDeviceContext()->OMSetRenderTargets(1, &this->renderTarget, this->m_DepthStencilView);
		}
	}

	void D3D11RenderApi::setRasterizerState(D3D11RasterizerState* rasterizer)
	{
		context->getDeviceContext()->RSSetState(rasterizer->GetRasterizerState());
	}

	void D3D11RenderApi::setBlendState(D3D11BlendState* blend, float* factor, unsigned int mask)
	{
		context->getDeviceContext()->OMSetBlendState(blend != nullptr ? blend->GetBlendState() : 0, factor, mask);
	}

	void D3D11RenderApi::setInputLayout(InputLayout* layout)
	{
		D3D11InputLayout* inputLayout = static_cast<D3D11InputLayout*>(layout);
		context->getDeviceContext()->IASetInputLayout(inputLayout->m_InputLayout);
	}

	void D3D11RenderApi::drawIndexed(UINT count)
	{
		context->getDeviceContext()->DrawIndexed(count, 0, 0);
	}

	void D3D11RenderApi::setGpuBuffer(GPUBuffer* buffer)
	{
		D3D11GPUBuffer* gpuBuffer = static_cast<D3D11GPUBuffer*>(buffer);
		context->getDeviceContext()->VSSetConstantBuffers(gpuBuffer->getLocation(), 1, &gpuBuffer->m_Buffer);
		context->getDeviceContext()->PSSetConstantBuffers(gpuBuffer->getLocation(), 1, &gpuBuffer->m_Buffer);
		context->getDeviceContext()->HSSetConstantBuffers(gpuBuffer->getLocation(), 1, &gpuBuffer->m_Buffer);
		context->getDeviceContext()->DSSetConstantBuffers(gpuBuffer->getLocation(), 1, &gpuBuffer->m_Buffer);
	}

	void D3D11RenderApi::setViewport(int w0, int y0, int w, int y)
	{
		vp.Width = (float)w;
		vp.Height = (float)y;
		vp.TopLeftX = w0;
		vp.TopLeftY = y0;

		context->getDeviceContext()->RSSetViewports(1, &vp);
	}

	void D3D11RenderApi::draw(UINT count)
	{
		context->getDeviceContext()->Draw(count, 0);
	}

	void D3D11RenderApi::present()
	{
		context->getSwapChain()->Present(1, 0);
	}
}