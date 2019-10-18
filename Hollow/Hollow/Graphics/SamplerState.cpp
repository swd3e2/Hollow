#include "SamplerState.h"
#include "RenderStateManager.h"

namespace Hollow {
	s_ptr<SamplerState> SamplerState::create(const SAMPLER_STATE_DESC& desc)
	{
		return RenderStateManager::instance()->createSamplerState(desc);
	}
}

