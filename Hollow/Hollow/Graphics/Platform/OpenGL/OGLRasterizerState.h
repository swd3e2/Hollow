#pragma once

#include "Hollow/Graphics/Base/RasterizerState.h"

namespace Hollow {
	class OGLRasterizerState : public RasterizerState
	{
	private:
		friend class OGLRenderApi;
		RASTERIZER_STATE_DESC desc;
	public:
		OGLRasterizerState(const RASTERIZER_STATE_DESC& desc) :
			desc(desc)
		{}
	};
}