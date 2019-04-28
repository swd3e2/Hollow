#include "D3D11RasterizerState.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

D3D11RasterizerState::D3D11RasterizerState()
{
	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	rasterizerDesc.DepthBias = 0.0001f;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 2.0f;

	D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
	ID3D11Device* device = r->getContext().getDevice();

	hr = device->CreateRasterizerState(&rasterizerDesc, &m_RasterizerState);
	
	if (hr != S_OK) {
		//HW_ERROR("RenderSystem: Cant create RasterizerState!");
	}
}

ID3D11RasterizerState* D3D11RasterizerState::GetRasterizerState()
{
	return m_RasterizerState;
}
