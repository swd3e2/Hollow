#include "RenderTarget.h"
#include "Hollow/Graphics/RenderTargetManager.h"

namespace Hollow {
	RenderTarget* RenderTarget::create(RENDER_TARGET_DESC desc)
	{ 
		return RenderTargetManager::instance()->create(desc); 
	}
}

