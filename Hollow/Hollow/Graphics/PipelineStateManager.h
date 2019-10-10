#pragma once

#include "Hollow/Core/CModule.h"
#include "PipelineState.h"
#include "DepthStencil.h"

namespace Hollow {
	class PipelineStateManager : public CModule<PipelineStateManager>
	{
	public:
		virtual PipelineState* create(const PIPELINE_STATE_DESC& desc) = 0;
	};
}