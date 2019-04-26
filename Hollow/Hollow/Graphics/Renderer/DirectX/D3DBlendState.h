#pragma once
#include <d3d11.h>
#include "Hollow/Common/Log.h"
#include "Hollow/Platform.h"
#include "D3DRenderer.h"

class D3DBlendState
{
private:
	ID3D11BlendState* m_BlendState;
public:
	D3DBlendState()
	{
		HRESULT hr = S_OK;

		D3DContext context = D3DRenderer::instance()->getContext();

		D3D11_RENDER_TARGET_BLEND_DESC blendingTragetDesc = { 0 };

		blendingTragetDesc.BlendEnable = true;
		blendingTragetDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendingTragetDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendingTragetDesc.BlendOp = D3D11_BLEND_OP_ADD;
		blendingTragetDesc.SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendingTragetDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		blendingTragetDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendingTragetDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		D3D11_BLEND_DESC blendDesc = { 0 };
		
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.RenderTarget[0] = blendingTragetDesc;

		hr = context.device->CreateBlendState(&blendDesc, &m_BlendState);

		if (hr != S_OK) {
			HW_ERROR("RenderSystem: Cant create blend state!");
		}
	}

	~D3DBlendState()
	{
		SAFE_RELEASE(m_BlendState);
	}

	ID3D11BlendState* GetBlendState() 
	{
		return m_BlendState;
	}
};