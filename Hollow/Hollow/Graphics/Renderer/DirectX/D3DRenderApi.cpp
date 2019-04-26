#include "D3DRenderApi.h"
#include "D3DContext.h"

D3DRenderApi::D3DRenderApi(int width, int height, HWND* hwnd)
{
	HRESULT hr = S_OK;
	RECT rc;

	context = new D3DContext();

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

	m_rasterizerState = new D3DRasterizerState();
	context->deviceContext->RSSetState(m_rasterizerState->GetRasterizerState());
	setStartedUp();
}

D3DRenderApi::~D3DRenderApi()
{
	setShutdown();
}

void D3DRenderApi::SetShaderResource(UINT slot, ID3D11ShaderResourceView* shaderResourceView)
{
	context->deviceContext->PSSetShaderResources(slot, 1, &shaderResourceView);
}

void D3DRenderApi::FreeShaderResource(UINT slot)
{
	context->deviceContext->PSSetShaderResources(slot, 1, pSRV);
}

void D3DRenderApi::SetVertexShader(D3DVertexShader* vs)
{
	context->deviceContext->VSSetShader(vs->GetShader(), NULL, 0);
	context->deviceContext->IASetInputLayout(vs->GetInputLayout());
}

void D3DRenderApi::SetPixelShader(D3DPixelShader* ps)
{
	context->deviceContext->PSSetShader(ps->GetShader(), NULL, 0);
}

void D3DRenderApi::ClearDepthStencilView(D3DDepthStencil* ds, int flag)
{
	context->deviceContext->ClearDepthStencilView(ds->GetDepthStencilView(), flag, 1.0f, 0);
}

void D3DRenderApi::SetContantBuffer(UINT slot, D3DConstantBuffer* cb)
{
	context->deviceContext->VSSetConstantBuffers(slot, 1, cb->GetAddressOf());
	context->deviceContext->PSSetConstantBuffers(slot, 1, cb->GetAddressOf());
}

void D3DRenderApi::SetVertexBuffer(IVertexBuffer* buffer)
{
	D3DVertexBuffer* buff = static_cast<D3DVertexBuffer*>(buffer);
	context->deviceContext->IASetVertexBuffers(0, 1, buff->GetAddressOf(), buff->StridePtr(), &this->offset);
}

void D3DRenderApi::ClearRenderTargetView(D3DRenderTarget* renderTarget, float* color)
{
	context->deviceContext->ClearRenderTargetView(renderTarget->GetRenderTaget(), color);
}

void D3DRenderApi::SetSampler(int slot, D3DSamplerState* sampler)
{
	context->deviceContext->PSSetSamplers(slot, 1, sampler->GetSamplerState());
}

void D3DRenderApi::SetDepthStencil(D3DDepthStencil* depthStencil)
{
	context->deviceContext->OMSetDepthStencilState(depthStencil->GetDepthStencilState(), 0);
}

void D3DRenderApi::SetRenderTarget(D3DRenderTarget* renderTarget, D3DDepthStencil* depthStencil)
{
	context->deviceContext->OMSetRenderTargets(1, renderTarget->GetAddressOfRenderTaget(), depthStencil->GetDepthStencilView());
}

void D3DRenderApi::SetViewPort(int numViewPorts, D3D11_VIEWPORT* vp)
{
	context->deviceContext->RSSetViewports(1, vp);
}

void D3DRenderApi::SetRasterizerState(D3DRasterizerState* rasterizer)
{
	context->deviceContext->RSSetState(rasterizer->GetRasterizerState());
}

void D3DRenderApi::SwapBuffers()
{
	context->swapChain->Present(vSync, 0);
}

void D3DRenderApi::SetBlendState(D3DBlendState* blend, float* factor, unsigned int mask)
{
	context->deviceContext->OMSetBlendState(blend != nullptr ? blend->GetBlendState() : 0, factor, mask);
}

void D3DRenderApi::DrawIndexed(UINT count)
{
	context->deviceContext->DrawIndexed(count, 0, 0);
}

void D3DRenderApi::Draw(UINT count)
{
	context->deviceContext->Draw(count, 0);
}

D3DContext* D3DRenderApi::getContext()
{
	return context;
}
