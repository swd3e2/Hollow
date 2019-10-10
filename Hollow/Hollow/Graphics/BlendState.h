#pragma once

#include "CommonTypes.h"
#include <stdint.h>

namespace Hollow {
	struct RENDER_TARGET_BLEND_STATE
	{
		bool			blendEnabled;
		BlendFunction	srcBlend;
		BlendFunction	dstBlend;
		BlendOperation	blendOp;
		BlendFunction	srcBlendAlpha;
		BlendFunction	dstBlendAlpha;
		BlendOperation	blendOpAlpha;
		uint8_t			writeMask;
	};

	struct BLEND_STATE_DESC
	{
		bool						alphaToCoverageEnable;
		bool						independentBlendEnable;
		RENDER_TARGET_BLEND_STATE	blend[8];
	};

	class BlendState
	{

	};
}