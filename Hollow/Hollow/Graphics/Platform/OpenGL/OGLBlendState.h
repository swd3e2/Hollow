#pragma once

#include "Hollow/Graphics/Base/BlendState.h"
#include "OGLPrerequisites.h"

namespace Hollow {
	class OGLBlendState : public BlendState
	{
	private:
		friend class OGLRenderApi;
		BLEND_STATE_DESC desc;
	public:
		OGLBlendState(const BLEND_STATE_DESC& desc) : 
			desc(desc)
		{}
	};
}