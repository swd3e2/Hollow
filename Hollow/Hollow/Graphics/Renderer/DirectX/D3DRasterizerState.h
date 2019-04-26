#pragma once
#include <d3d11.h>
#include "Hollow/Common/Log.h"
#include "D3DRenderer.h"

class D3DRasterizerState
{
private:
	ID3D11RasterizerState* m_RasterizerState;
public:
	D3DRasterizerState()
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

		hr = D3DRenderer::instance()->getContext().device->CreateRasterizerState(&rasterizerDesc, &m_RasterizerState);

		if (hr != S_OK) {
			HW_ERROR("RenderSystem: Cant create RasterizerState!");
		}
	}

	ID3D11RasterizerState* GetRasterizerState()
	{
		return m_RasterizerState;
	}
};