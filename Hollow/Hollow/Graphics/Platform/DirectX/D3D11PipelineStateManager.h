#pragma once

#include "D3D11PipelineState.h"
#include "Hollow/Graphics/PipelineStateManager.h"

namespace Hollow {
	class D3D11PipelineStateManager : public PipelineStateManager
	{
	public:
		virtual PipelineState* create(const PIPELINE_STATE_DESC& desc) override;
	};
}