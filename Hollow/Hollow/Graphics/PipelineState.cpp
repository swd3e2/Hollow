#include "PipelineState.h"
#include "Hollow/Graphics/PipelineStateManager.h"

namespace Hollow {
	PipelineState* PipelineState::create(const PIPELINE_STATE_DESC& desc)
	{
		return PipelineStateManager::instance()->create(desc);
	}
}

