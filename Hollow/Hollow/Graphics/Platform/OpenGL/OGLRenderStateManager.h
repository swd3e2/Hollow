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
		virtual SamplerState*		createSamplerState(const SAMPLER_STATE_DESC& desc) override;
		virtual RasterizerState*	createRasterizerState(const RASTERIZER_STATE_DESC& desc) override;
		virtual BlendState*			createBlendState(const BLEND_STATE_DESC& desc) override;
		virtual DepthStencil*		createDepthStencilState(const DEPTH_STENCIL_STATE_DESC& desc) override;
	};
}