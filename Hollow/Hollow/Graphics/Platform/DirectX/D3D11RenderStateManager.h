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
		virtual SamplerState*		createSamplerState(const SAMPLER_STATE_DESC& desc) override;
		virtual RasterizerState*	createRasterizerState(const RASTERIZER_STATE_DESC& desc) override;
		virtual BlendState*			createBlendState(const BLEND_STATE_DESC& desc) override;
		virtual DepthStencil*		createDepthStencilState(const DEPTH_STENCIL_STATE_DESC& desc) override;
	};
}