#pragma once

#include "D3D11PipelineState.h"
#include "D3D11SamplerState.h"
#include "Hollow/Graphics/PipelineStateManager.h"
#include "D3D11Helper.h"

namespace Hollow {
	class D3D11PipelineStateManager : public PipelineStateManager
	{
	public:
		virtual PipelineState* create(const PIPELINE_STATE_DESC& desc) override;
		virtual SamplerState* create(const SAMPLER_STATE_DESC& desc) override;
	};
}