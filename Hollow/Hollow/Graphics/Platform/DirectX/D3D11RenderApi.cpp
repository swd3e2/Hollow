#include "D3D11RenderApi.h"
#include "D3D11Context.h"
#include "D3D11RenderTarget.h"
#include "D3D11BlendState.h"
#include "D3D11SamplerState.h"
#include "D3D11RasterizerState.h"
#include "D3D11Context.h"
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
#include "D3D11InputLayoutManager.h"
#include "D3D11Win32Window.h"

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
		swapChainDesc.OutputWindow = (static_cast<D3D11Win32Window*>(Window::instance())->getHWND());

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

		hr = device->CreateRenderTargetView(backBuffer, NULL, &m_RenderTarget);

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

		blendState = new D3D11BlendState();

		HardwareBufferManager::startUp<D3D11HardwareBufferManager>();
		TextureManager::startUp<D3D11TextureManager>();
		ShaderManager::startUp<D3D11ShaderManager>();
		GPUBufferManager::startUp<D3D11GPUBufferManager>();
		RenderTargetManager::startUp<D3D11RenderTargetManager>();
		InputLayoutManager::startUp<D3D11InputLayoutManager>();
		RenderStateManager::startUp<D3D11RenderStateManager>();
	}

	void D3D11RenderApi::setTexture(UINT slot, const s_ptr<Texture>& texture)
	{
		D3D11Texture* d3dTexture = std::static_pointer_cast<D3D11Texture>(texture).get();
		if (texture != nullptr && d3dTexture->m_TextureShaderResource != nullptr) {
			context->getDeviceContext()->PSSetShaderResources(slot, 1, &d3dTexture->m_TextureShaderResource);
			context->getDeviceContext()->DSSetShaderResources(slot, 1, &d3dTexture->m_TextureShaderResource);
			//context->getDeviceContext()->CSGetUnorderedAccessViews(0, 1, &d3dTexture->m_UnorderedAccessView);
		} else {
			context->getDeviceContext()->PSSetShaderResources(slot, 1, pSRV);
			context->getDeviceContext()->DSSetShaderResources(slot, 1, pSRV);
			//context->getDeviceContext()->CSGetUnorderedAccessViews(slot, uavs, pUAV);
		}
	}

	void D3D11RenderApi::unsetTexture(UINT slot)
	{
		context->getDeviceContext()->PSSetShaderResources(slot, 1, pSRV);
		context->getDeviceContext()->DSSetShaderResources(slot, 1, pSRV);
	}

	void D3D11RenderApi::setTextureColorBuffer(UINT slot, const s_ptr<RenderTarget>& renderTarget, UINT targetNum)
	{
		D3D11RenderTarget* d3dRenderTarget = std::static_pointer_cast<D3D11RenderTarget>(renderTarget).get();
		context->getDeviceContext()->PSSetShaderResources(slot, 1, &d3dRenderTarget->getShaderResourceView()[targetNum]);
	}

	void D3D11RenderApi::setTextureDepthBuffer(UINT slot, const s_ptr<RenderTarget>& renderTarget)
	{
		D3D11RenderTarget* d3dRenderTarget = std::static_pointer_cast<D3D11RenderTarget>(renderTarget).get();
		auto ptr = d3dRenderTarget->getDepthStencilResource();
		context->getDeviceContext()->PSSetShaderResources(slot, 1, &ptr);
	}

	void D3D11RenderApi::setIndexBuffer(const s_ptr<IndexBuffer>& buffer)
	{
		s_ptr<D3D11IndexBuffer> buff = std::static_pointer_cast<D3D11IndexBuffer>(buffer);
		context->getDeviceContext()->IASetIndexBuffer(std::static_pointer_cast<D3D11HardwareBuffer>(buff->mHardwareBuffer)->get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void D3D11RenderApi::setVertexBuffer(const s_ptr<VertexBuffer>& buffer)
	{
		s_ptr<D3D11VertexBuffer> buff = std::static_pointer_cast<D3D11VertexBuffer>(buffer);
		context->getDeviceContext()->IASetVertexBuffers(0, 1, 
			std::static_pointer_cast<D3D11HardwareBuffer>(buff->mHardwareBuffer)->getAddressOf(), 
			std::static_pointer_cast<D3D11HardwareBuffer>(buff->mHardwareBuffer)->getStridePtr(),
			&this->offset
		);
	}

	void D3D11RenderApi::clearRenderTarget(const s_ptr<RenderTarget>& renderTarget, const float* color)
	{
		if (renderTarget != nullptr) {
			D3D11RenderTarget* d3d11RenderTarget = std::static_pointer_cast<D3D11RenderTarget>(renderTarget).get();
			for (int i = 0; i < d3d11RenderTarget->count; i++) {
				context->getDeviceContext()->ClearRenderTargetView(d3d11RenderTarget->getRenderTaget()[i], color);
			}
			context->getDeviceContext()->ClearDepthStencilView(d3d11RenderTarget->getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		} else {
			context->getDeviceContext()->ClearRenderTargetView(this->m_RenderTarget, color);
			context->getDeviceContext()->ClearDepthStencilView(this->m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}

	void D3D11RenderApi::setRenderTarget(const s_ptr<RenderTarget>& renderTarget)
	{
		if (renderTarget != nullptr) {
			D3D11RenderTarget* d3d11RenderTarget = std::static_pointer_cast<D3D11RenderTarget>(renderTarget).get();
			context->getDeviceContext()->OMSetRenderTargets(d3d11RenderTarget->count, d3d11RenderTarget->getRenderTaget(), d3d11RenderTarget->getDepthStencilView());
		} else {
			context->getDeviceContext()->OMSetRenderTargets(3, nullRTV, NULL);
			context->getDeviceContext()->OMSetRenderTargets(1, &this->m_RenderTarget, this->m_DepthStencilView);
		}
	}

	void D3D11RenderApi::setSampler(const int slot, const s_ptr<SamplerState>& sampler)
	{
		ID3D11SamplerState** samplerState = std::static_pointer_cast<D3D11SamplerState>(sampler)->getSamplerState();
		context->getDeviceContext()->PSSetSamplers(slot, 1, samplerState);
		context->getDeviceContext()->DSSetSamplers(slot, 1, samplerState);
	}

	void D3D11RenderApi::setTextureSampler(const int textureUnit, const s_ptr<SamplerState>& sampler)
	{
		
	}

	void D3D11RenderApi::setDepthStencilState(const s_ptr<DepthStencil>& depthStencil)
	{
		ID3D11DepthStencilState* state = std::static_pointer_cast<D3D11DepthStencil>(depthStencil)->depthStencilState;
		context->getDeviceContext()->OMSetDepthStencilState(state, 0xFF);
	}

	void D3D11RenderApi::setRasterizerState(const s_ptr<RasterizerState>& rasterizerState)
	{
		ID3D11RasterizerState* rasterizer = std::static_pointer_cast<D3D11RasterizerState>(rasterizerState)->getRasterizerState();
		context->getDeviceContext()->RSSetState(rasterizer);
	}

	void D3D11RenderApi::setBlendState(const s_ptr<BlendState>& blendState)
	{
		ID3D11BlendState* blend = std::static_pointer_cast<D3D11BlendState>(blendState)->getBlendState();
		context->getDeviceContext()->OMSetBlendState(blend, nullptr, 0xFFFFFFFF);
	}	

	void D3D11RenderApi::setShaderPipeline(const s_ptr<ShaderPipeline>& shaderPipeline)
	{
		D3D11ShaderPipeline* d3dPipeline = std::static_pointer_cast<D3D11ShaderPipeline>(shaderPipeline).get();

		const s_ptr<D3D11Shader>& vertexShader = std::static_pointer_cast<D3D11Shader>(d3dPipeline->getVertexShader());
		if (vertexShader != nullptr) {
			ID3D11VertexShader* d3dVertexShader = static_cast<ID3D11VertexShader*>(vertexShader->getShader());
			if (d3dVertexShader != nullptr) {
				context->getDeviceContext()->VSSetShader(d3dVertexShader, NULL, 0);
			}
		} else {
			context->getDeviceContext()->VSSetShader(nullptr, NULL, 0);
		}

		const s_ptr<D3D11Shader>& pixelShader = std::static_pointer_cast<D3D11Shader>(d3dPipeline->getPixelShader());
		if (pixelShader != nullptr) {
			ID3D11PixelShader* d3dPixelShader = static_cast<ID3D11PixelShader*>(pixelShader->getShader());
			if (d3dPixelShader != nullptr) {
				context->getDeviceContext()->PSSetShader(d3dPixelShader, NULL, 0);
			}
		} else {
			context->getDeviceContext()->PSSetShader(nullptr, NULL, 0);
		}

		const s_ptr<D3D11Shader>& geometryShader = std::static_pointer_cast<D3D11Shader>(d3dPipeline->getGeometryShader());
		if (geometryShader != nullptr) {
			ID3D11GeometryShader* d3dGeometryShader = static_cast<ID3D11GeometryShader*>(geometryShader->getShader());
			if (d3dGeometryShader != nullptr) {
				context->getDeviceContext()->GSSetShader(d3dGeometryShader, NULL, 0);
			}
		} else {
			context->getDeviceContext()->GSSetShader(nullptr, NULL, 0);
		}

		const s_ptr<D3D11Shader>& hullShader = std::static_pointer_cast<D3D11Shader>(d3dPipeline->getHullShader());
		if (hullShader != nullptr) {
			ID3D11HullShader* d3dHullShader = static_cast<ID3D11HullShader*>(hullShader->getShader());
			if (d3dHullShader != nullptr) {
				context->getDeviceContext()->HSSetShader(d3dHullShader, NULL, 0);
			}
		} else {
			context->getDeviceContext()->HSSetShader(nullptr, NULL, 0);
		}

		const s_ptr<D3D11Shader>& domainShader = std::static_pointer_cast<D3D11Shader>(d3dPipeline->getDomainShader());
		if (domainShader != nullptr) {
			ID3D11DomainShader* d3dDomainShader = static_cast<ID3D11DomainShader*>(domainShader->getShader());
			if (d3dDomainShader != nullptr) {
				context->getDeviceContext()->DSSetShader(d3dDomainShader, NULL, 0);
			}
		} else {
			context->getDeviceContext()->DSSetShader(nullptr, NULL, 0);
		}
	}

	void D3D11RenderApi::setPrimitiveTopology(const PrimitiveTopology topology)
	{
		if (topology == mTopology) return;

		mTopology = topology;

		switch (topology)
		{
		case PrimitiveTopology::PT_POINT:
			context->getDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
			break;
		case PrimitiveTopology::PT_LINESTRIP:
			context->getDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
			break;
		case PrimitiveTopology::PT_LINELIST:
			context->getDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		case PrimitiveTopology::PT_TRIANGELIST:
			context->getDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		case PrimitiveTopology::PT_TRIANGESTRIP:
			context->getDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			break;
		default:
			break;
		}
	}

	void D3D11RenderApi::drawInstanced(UINT count, UINT instanceCount)
	{
		context->getDeviceContext()->DrawInstanced(count, instanceCount, 0, 0);
	}

	void D3D11RenderApi::drawIndexedInstanced(UINT count, UINT instanceCount)
	{
		context->getDeviceContext()->DrawIndexedInstanced(count, instanceCount, 0, 0, 0);
	}

	void D3D11RenderApi::setInputLayout(const s_ptr<InputLayout>& layout)
	{
		s_ptr<D3D11InputLayout> inputLayout = std::static_pointer_cast<D3D11InputLayout>(layout);
		context->getDeviceContext()->IASetInputLayout(inputLayout->m_InputLayout);
	}

	void D3D11RenderApi::drawIndexed(UINT count)
	{
		context->getDeviceContext()->DrawIndexed(count, 0, 0);
	}

	void D3D11RenderApi::setGpuBuffer(const s_ptr<GPUBuffer>& buffer)
	{
		s_ptr<D3D11GPUBuffer> gpuBuffer = std::static_pointer_cast<D3D11GPUBuffer>(buffer);
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
		context->getSwapChain()->Present(false, 0);
	}
}