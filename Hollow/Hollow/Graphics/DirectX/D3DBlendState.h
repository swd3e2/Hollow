#pragma once
#include <d3d11.h>
#include "Hollow/Common/Log.h"
#include "Hollow/Platform.h"

class D3DBlendState
{
private:
	ID3D11BlendState* m_BlendState;
public:
	D3DBlendState(ID3D11Device* device)
	{
		HRESULT hr = S_OK;

		D3D11_RENDER_TARGET_BLEND_DESC blendingTragetDesc = { 0 };

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

		hr = device->CreateBlendState(&blendDesc, &m_BlendState);

		if (hr != S_OK) {
			HW_ERROR("RenderSystem: Cant create blend state!");
		}
	}

	~D3DBlendState()
	{
		SAFE_RELEASE(m_BlendState);
	}

	ID3D11BlendState* const * GetBlendState() 
	{
		return &m_BlendState;
	}
};