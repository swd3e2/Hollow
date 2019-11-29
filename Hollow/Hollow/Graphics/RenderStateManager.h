#pragma once

#include "SamplerState.h"
#include "RasterizerState.h"
#include "BlendState.h"
#include "DepthStencil.h"
#include "Hollow/Core/CModule.h"

namespace Hollow {
	class SamplerState;
	class RasterizerState;
	class BlendState;
	class DepthStencil;

	class RenderStateManager : public CModule<RenderStateManager>
	{
	public:
		virtual s_ptr<SamplerState>		createSamplerState(const SAMPLER_STATE_DESC& desc) = 0;
		virtual s_ptr<RasterizerState>	createRasterizerState(const RASTERIZER_STATE_DESC& desc) = 0;
		virtual s_ptr<BlendState>		createBlendState(const BLEND_STATE_DESC& desc) = 0;
		virtual s_ptr<DepthStencil>		createDepthStencilState(const DEPTH_STENCIL_STATE_DESC& desc) = 0;
	};
}