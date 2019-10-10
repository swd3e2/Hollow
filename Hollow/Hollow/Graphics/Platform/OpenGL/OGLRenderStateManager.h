#pragma once

#include "Hollow/Graphics/RenderStateManager.h"
#include "OGLSamplerState.h"
#include "OGLDepthStencilState.h"
#include "OGLRasterizerState.h"
#include "OGLHelper.h"
#include "OGLBlendState.h"

namespace Hollow {
	class OGLRenderStateManager : public RenderStateManager
	{
	public:
		virtual s_ptr<SamplerState>		createSamplerState(const SAMPLER_STATE_DESC& desc) override;
		virtual s_ptr<RasterizerState>	createRasterizerState(const RASTERIZER_STATE_DESC& desc) override;
		virtual s_ptr<BlendState>		createBlendState(const BLEND_STATE_DESC& desc) override;
		virtual s_ptr<DepthStencil>		createDepthStencilState(const DEPTH_STENCIL_STATE_DESC& desc) override;
	};
}