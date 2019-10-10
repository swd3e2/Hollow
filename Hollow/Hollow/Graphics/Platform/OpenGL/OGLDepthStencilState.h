#pragma once

#include "Hollow/Graphics/DepthStencil.h"
#include "OGLPrerequisites.h"

namespace Hollow {
	class OGLDepthStencilState : public DepthStencil
	{
	private:
		friend class OGLRenderApi;
		DEPTH_STENCIL_STATE_DESC desc;
	public:
		OGLDepthStencilState(const DEPTH_STENCIL_STATE_DESC& desc) :
			desc(desc)
		{}
	};
}