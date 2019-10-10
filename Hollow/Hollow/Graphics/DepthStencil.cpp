#include "DepthStencil.h"
#include "Hollow/Graphics/RenderStateManager.h"

namespace Hollow {
	DepthStencil* DepthStencil::create(const DEPTH_STENCIL_STATE_DESC& desc)
	{
		return RenderStateManager::instance()->createDepthStencilState(desc);
	}
}

