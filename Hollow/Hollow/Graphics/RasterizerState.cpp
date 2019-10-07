#include "RasterizerState.h"
#include "Hollow/Graphics/RenderStateManager.h"

namespace Hollow {
	RasterizerState* RasterizerState::create(const RASTERIZER_STATE_DESC& desc)
	{
		return RenderStateManager::instance()->createRasterizerState(desc);
	}
}

