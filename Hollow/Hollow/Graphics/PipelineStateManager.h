#pragma once

#include "Hollow/Core/CModule.h"
#include "Base/PipelineState.h"
#include "Base/SamplerState.h"
#include "Base/DepthStencil.h"

namespace Hollow {
	class PipelineStateManager : public CModule<PipelineStateManager>
	{
	public:
		virtual PipelineState* create(const PIPELINE_STATE_DESC& desc) = 0;
		virtual SamplerState* create(const SAMPLER_STATE_DESC& desc) = 0;
	};
}