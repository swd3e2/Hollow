#pragma once

#include "Hollow/Graphics/RenderStateManager.h"
#include "D3D11Prerequisites.h"
#include "D3D11Helper.h"
#include "D3D11DepthStencil.h"
#include "D3D11SamplerState.h"
#include "D3D11RasterizerState.h"
#include "D3D11BlendState.h"

namespace Hollow {
	class D3D11RenderStateManager : public RenderStateManager
	{
		virtual s_ptr<SamplerState>		createSamplerState(const SAMPLER_STATE_DESC& desc) override;
		virtual s_ptr<RasterizerState>	createRasterizerState(const RASTERIZER_STATE_DESC& desc) override;
		virtual s_ptr<BlendState>		createBlendState(const BLEND_STATE_DESC& desc) override;
		virtual s_ptr<DepthStencil>		createDepthStencilState(const DEPTH_STENCIL_STATE_DESC& desc) override;
	};
}