#include "D3D11RenderApi.h"
#include "D3D11Context.h"
#include "D3D11RenderTarget.h"
#include "D3D11DepthStencil.h"
#include "D3D11PixelShader.h"
#include "D3D11VertexShader.h"
#include "D3D11ConstantBuffer.h"
#include "D3D11Renderable.h"
#include "D3D11BlendState.h"
#include "D3D11SamplerState.h"
#include "D3D11RasterizerState.h"
#include "D3D11Material.h"
#include "D3D11Context.h"
#include "D3D11ConstBufferMapping.h"
#include "D3D11RenderTarget.h"
#include "D3D11VertexBuffer.h"

D3D11RenderApi::D3D11RenderApi(int width, int height, HWND* hwnd)
{
	HRESULT hr = S_OK;
	RECT rc;

	context = new D3D11Context();

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
		featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapChainDesc, &context->swapChain,
		&context->device, &m_featureLevel, &context->deviceContext);

	if (hr != S_OK) {
		HW_ERROR("RenderSystem: Can't create DeviceAndSwapChain!");
	}
		
	context->deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_rasterizerState = new D3D11RasterizerState();
	context->deviceContext->RSSetState(m_rasterizerState->GetRasterizerState());
	setStartedUp();
}

D3D11RenderApi::~D3D11RenderApi()
{
	setShutdown();
}

void D3D11RenderApi::SetShaderResource(UINT slot, ID3D11ShaderResourceView* shaderResourceView)
{
	context->deviceContext->PSSetShaderResources(slot, 1, &shaderResourceView);
}

void D3D11RenderApi::FreeShaderResource(UINT slot)
{
	context->deviceContext->PSSetShaderResources(slot, 1, pSRV);
}

void D3D11RenderApi::SetVertexShader(D3D11VertexShader* vs)
{
	context->deviceContext->VSSetShader(vs->GetShader(), NULL, 0);
	context->deviceContext->IASetInputLayout(vs->GetInputLayout());
}

void D3D11RenderApi::SetPixelShader(D3D11PixelShader* ps)
{
	context->deviceContext->PSSetShader(ps->GetShader(), NULL, 0);
}

void D3D11RenderApi::ClearDepthStencilView(D3D11DepthStencil* ds, int flag)
{
	context->deviceContext->ClearDepthStencilView(ds->GetDepthStencilView(), flag, 1.0f, 0);
}

void D3D11RenderApi::SetContantBuffer(UINT slot, D3D11ConstantBuffer* cb)
{
	context->deviceContext->VSSetConstantBuffers(slot, 1, cb->GetAddressOf());
	context->deviceContext->PSSetConstantBuffers(slot, 1, cb->GetAddressOf());
}

void D3D11RenderApi::SetVertexBuffer(VertexBuffer* buffer)
{
	D3D11VertexBuffer* buff = (D3D11VertexBuffer*)(buffer);
	context->deviceContext->IASetVertexBuffers(0, 1, buff->GetAddressOf(), buff->StridePtr(), &this->offset);
}

void D3D11RenderApi::ClearRenderTargetView(D3D11RenderTarget* renderTarget, float* color)
{
	context->deviceContext->ClearRenderTargetView(renderTarget->GetRenderTaget(), color);
}

void D3D11RenderApi::SetSampler(int slot, D3D11SamplerState* sampler)
{
	context->deviceContext->PSSetSamplers(slot, 1, sampler->GetSamplerState());
}

void D3D11RenderApi::SetDepthStencil(D3D11DepthStencil* depthStencil)
{
	context->deviceContext->OMSetDepthStencilState(depthStencil->GetDepthStencilState(), 0);
}

void D3D11RenderApi::SetRenderTarget(D3D11RenderTarget* renderTarget, D3D11DepthStencil* depthStencil)
{
	context->deviceContext->OMSetRenderTargets(1, renderTarget->GetAddressOfRenderTaget(), depthStencil->GetDepthStencilView());
}

void D3D11RenderApi::SetViewPort(int numViewPorts, D3D11_VIEWPORT* vp)
{
	context->deviceContext->RSSetViewports(1, vp);
}

void D3D11RenderApi::SetRasterizerState(D3D11RasterizerState* rasterizer)
{
	context->deviceContext->RSSetState(rasterizer->GetRasterizerState());
}

void D3D11RenderApi::SwapBuffers()
{
	context->swapChain->Present(vSync, 0);
}

void D3D11RenderApi::SetBlendState(D3D11BlendState* blend, float* factor, unsigned int mask)
{
	context->deviceContext->OMSetBlendState(blend != nullptr ? blend->GetBlendState() : 0, factor, mask);
}

void D3D11RenderApi::DrawIndexed(UINT count)
{
	context->deviceContext->DrawIndexed(count, 0, 0);
}

void D3D11RenderApi::Draw(UINT count)
{
	context->deviceContext->Draw(count, 0);
}

D3D11Context* D3D11RenderApi::getContext()
{
	return context;
}
