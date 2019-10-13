#include "RasterizerState.h"
#include "Hollow/Graphics/RenderStateManager.h"

namespace Hollow {
	s_ptr<RasterizerState> RasterizerState::create(const RASTERIZER_STATE_DESC& desc)
	{
		return RenderStateManager::instance()->createRasterizerState(desc);
	}
}

