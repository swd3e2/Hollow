#pragma once

#include "Base/SamplerState.h"
#include "Base/RasterizerState.h"
#include "Base/BlendState.h"
#include "Base/DepthStencil.h"
#include "Hollow/Core/CModule.h"

namespace Hollow {
	class SamplerState;
	class RasterizerState;
	class BlendState;
	class DepthStencil;

	class RenderStateManager : public CModule<RenderStateManager>
	{
	public:
		virtual SamplerState*		createSamplerState(const SAMPLER_STATE_DESC& desc) = 0;
		virtual RasterizerState*	createRasterizerState(const RASTERIZER_STATE_DESC& desc) = 0;
		virtual BlendState*			createBlendState(const BLEND_STATE_DESC& desc) = 0;
		virtual DepthStencil*		createDepthStencilState(const DEPTH_STENCIL_STATE_DESC& desc) = 0;
	};
}