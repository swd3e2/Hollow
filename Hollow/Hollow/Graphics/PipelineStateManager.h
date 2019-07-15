#pragma once

#include "Hollow/Core/CModule.h"
#include "Base/PipelineState.h"
namespace Hollow {
	class PipelineStateManager : public CModule<PipelineStateManager>
	{
	public:
		virtual PipelineState* create(const PIPELINE_STATE_DESC& desc) = 0;
	};
}