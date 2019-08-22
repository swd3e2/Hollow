#pragma once

#include "CommonTypes.h"
#include <stdint.h>

namespace Hollow {
	struct DEPTH_STENCIL_STATE
	{
		bool depthEnable;
		CompareFunction depthCompareFunction;
		uint8_t depthWriteMask;
		bool stencilEnable;
		uint8_t stencilWriteMask;
		uint8_t stencilWriteMask;
	};

	class DepthStencil
	{
	public:
		static DepthStencil* create(const DEPTH_STENCIL_STATE& desc);
	};
}