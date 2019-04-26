#pragma once

#include "Hollow/Common/Log.h"
#include "Hollow/Platform.h"
#include "D3D11RenderApi.h"

class D3D11BlendState
{
private:
	ID3D11BlendState* m_BlendState;
public:
	D3D11BlendState()
	{
		HRESULT hr = S_OK;

		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11Device* device = r->getContext()->device;

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

		hr = device->CreateBlendState(&blendDesc, &m_BlendState);

		if (hr != S_OK) {
			HW_ERROR("RenderSystem: Cant create blend state!");
		}
	}

	~D3D11BlendState()
	{
		SAFE_RELEASE(m_BlendState);
	}

	ID3D11BlendState* GetBlendState() 
	{
		return m_BlendState;
	}
};