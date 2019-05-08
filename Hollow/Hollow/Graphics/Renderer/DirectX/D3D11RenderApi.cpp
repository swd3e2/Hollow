#include "D3D11RenderApi.h"
#include "D3D11Context.h"
#include "D3D11RenderTarget.h"
#include "D3D11DepthStencil.h"
#include "D3D11PixelShader.h"
#include "D3D11VertexShader.h"
#include "D3D11ConstantBuffer.h"
#include "D3D11BlendState.h"
#include "D3D11SamplerState.h"
#include "D3D11RasterizerState.h"
#include "D3D11Context.h"
#include "D3D11ConstBufferMapping.h"
#include "D3D11RenderTarget.h"
#include "D3D11VertexBuffer.h"
#include "D3D11TextureManager.h"
#include "D3D11HardwareBufferManager.h"
#include "D3D11ShaderManager.h"
#include "D3D11Shader.h"
#include "D3D11WindowManager.h"

D3D11RenderApi::D3D11RenderApi(int width, int height)
{
	windowManager = new D3D11WindowManager();
}

D3D11RenderApi::~D3D11RenderApi()
{
	setShutdown();
}

void D3D11RenderApi::startUp()
{
	HRESULT hr = S_OK;
	RECT rc;

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;

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
	swapChainDesc.OutputWindow = *(static_cast<D3D11Win32Window*>(WindowManager::instance()->getWindow())->getHWND());

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_1;
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags,
		featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
		&device, &m_featureLevel, &deviceContext);


	if (hr != S_OK) {
		//HW_ERROR("RenderSystem: Can't create DeviceAndSwapChain!");
	}

	context = new D3D11Context(device, deviceContext, swapChain);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	setStartedUp();

	m_rasterizerState = new D3D11RasterizerState();
	deviceContext->RSSetState(m_rasterizerState->GetRasterizerState());

	m_SamplerStateClamp = new D3D11SamplerState(D3D11_TEXTURE_ADDRESS_CLAMP);
	m_SamplerStateWrap = new D3D11SamplerState(D3D11_TEXTURE_ADDRESS_WRAP);

	SetSampler(0, m_SamplerStateClamp);
	SetSampler(1, m_SamplerStateWrap);

	textureManager = new D3D11TextureManager();
	hardwareBufferManager = new D3D11HardwareBufferManager();
	shaderManager = new D3D11ShaderManager();
}

void D3D11RenderApi::SetTexture(UINT slot, Texture* texture)
{
	D3D11Texture* d3dTexture = static_cast<D3D11Texture*>(texture);
	context->getDeviceContext()->PSSetShaderResources(slot, 1, &d3dTexture->m_TextureShaderResource);
}

void D3D11RenderApi::SetTexture(UINT slot, RenderTarget* renderTarget)
{
	D3D11RenderTarget* d3dRenderTarget = static_cast<D3D11RenderTarget*>(renderTarget);
	context->getDeviceContext()->PSSetShaderResources(slot, 1, d3dRenderTarget->GetAddressOfShaderResourceView());
}

void D3D11RenderApi::SetShader(Shader* shader)
{
	D3D11Shader* d3dShader = static_cast<D3D11Shader*>(shader);
	context->getDeviceContext()->VSSetShader(d3dShader->getVertexShader()->GetShader(), NULL, 0);
	context->getDeviceContext()->IASetInputLayout(d3dShader->getVertexShader()->GetInputLayout());
	context->getDeviceContext()->PSSetShader(d3dShader->getPixelShader()->GetShader(), NULL, 0);
}

void D3D11RenderApi::FreeShaderResource(UINT slot)
{
	context->getDeviceContext()->PSSetShaderResources(slot, 1, pSRV);
}

void D3D11RenderApi::SetVertexShader(D3D11VertexShader* vs)
{
	context->getDeviceContext()->VSSetShader(vs->GetShader(), NULL, 0);
	context->getDeviceContext()->IASetInputLayout(vs->GetInputLayout());
}

void D3D11RenderApi::SetPixelShader(D3D11PixelShader* ps)
{
	context->getDeviceContext()->PSSetShader(ps->GetShader(), NULL, 0);
}

void D3D11RenderApi::ClearDepthStencilView(D3D11DepthStencil* ds, int flag)
{
	context->getDeviceContext()->ClearDepthStencilView(ds->GetDepthStencilView(), flag, 1.0f, 0);
}

void D3D11RenderApi::SetContantBuffer(UINT slot, D3D11ConstantBuffer* cb)
{
	context->getDeviceContext()->VSSetConstantBuffers(slot, 1, cb->GetAddressOf());
	context->getDeviceContext()->PSSetConstantBuffers(slot, 1, cb->GetAddressOf());
}

void D3D11RenderApi::SetIndexBuffer(IndexBuffer* buffer)
{
	D3D11IndexBuffer* buff = (D3D11IndexBuffer*)(buffer);
	context->getDeviceContext()->IASetIndexBuffer(buff->Get(), DXGI_FORMAT_R32_UINT, 0);
}

void D3D11RenderApi::SetVertexBuffer(VertexBuffer* buffer)
{
	D3D11VertexBuffer* buff = (D3D11VertexBuffer*)(buffer);
	context->getDeviceContext()->IASetVertexBuffers(0, 1, buff->GetAddressOf(), buff->StridePtr(), &this->offset);
}

void D3D11RenderApi::ClearRenderTargetView(D3D11RenderTarget* renderTarget, float* color)
{
	context->getDeviceContext()->ClearRenderTargetView(renderTarget->GetRenderTaget(), color);
}

void D3D11RenderApi::SetSampler(int slot, D3D11SamplerState* sampler)
{
	context->getDeviceContext()->PSSetSamplers(slot, 1, sampler->GetSamplerState());
}

void D3D11RenderApi::SetDepthStencil(D3D11DepthStencil* depthStencil)
{
	context->getDeviceContext()->OMSetDepthStencilState(depthStencil->GetDepthStencilState(), 0);
}

void D3D11RenderApi::SetRenderTarget(D3D11RenderTarget* renderTarget, D3D11DepthStencil* depthStencil)
{
	context->getDeviceContext()->OMSetRenderTargets(1, renderTarget->GetAddressOfRenderTaget(), depthStencil->GetDepthStencilView());
	context->getDeviceContext()->OMSetDepthStencilState(depthStencil->GetDepthStencilState(), 0);
}

void D3D11RenderApi::SetViewPort(int numViewPorts, D3D11_VIEWPORT* vp)
{
	context->getDeviceContext()->RSSetViewports(1, vp);
}

void D3D11RenderApi::SetRasterizerState(D3D11RasterizerState* rasterizer)
{
	context->getDeviceContext()->RSSetState(rasterizer->GetRasterizerState());
}

void D3D11RenderApi::SwapBuffers()
{
	context->getSwapChain()->Present(vSync, 0);
}

void D3D11RenderApi::SetBlendState(D3D11BlendState* blend, float* factor, unsigned int mask)
{
	context->getDeviceContext()->OMSetBlendState(blend != nullptr ? blend->GetBlendState() : 0, factor, mask);
}

void D3D11RenderApi::DrawIndexed(UINT count)
{
	context->getDeviceContext()->DrawIndexed(count, 0, 0);
}

void D3D11RenderApi::Draw(UINT count)
{
	context->getDeviceContext()->Draw(count, 0);
}

void D3D11RenderApi::Present()
{
	context->getSwapChain()->Present(1,0);
}
