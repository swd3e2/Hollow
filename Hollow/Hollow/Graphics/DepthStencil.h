#pragma once

#include "CommonTypes.h"
#include <stdint.h>
#include "CommonTypes.h"
#include "Hollow/Platform.h"

namespace Hollow {
	struct DEPTH_STENCIL_STATEOP_DESC
	{
		StencilOperation failOp;
		StencilOperation depthFailOp;
		StencilOperation passOp;
		ComparisonFunction stencilFunc;

		DEPTH_STENCIL_STATEOP_DESC() :
			failOp(StencilOperation::SOP_KEEP), depthFailOp(StencilOperation::SOP_KEEP), passOp(StencilOperation::SOP_KEEP), 
			stencilFunc(ComparisonFunction::CMP_NEVER)
		{}
	};

	struct DEPTH_STENCIL_STATE_DESC
	{
		bool						depthEnable;
		ComparisonFunction			depthFunc;
		DepthWriteMask				depthWriteMask;
		bool						stencilEnable;
		uint8_t						stencilWriteMask;
		uint8_t						stencilReadMask;
		DEPTH_STENCIL_STATEOP_DESC	front;
		DEPTH_STENCIL_STATEOP_DESC	back;

		DEPTH_STENCIL_STATE_DESC() :
			depthEnable(true), depthFunc(ComparisonFunction::CMP_LESS), depthWriteMask(DepthWriteMask::DWM_ALL), 
			stencilEnable(false), stencilWriteMask(0xFF), stencilReadMask(0xFF)
		{}
	};

	class DepthStencil
	{
	public:
		static s_ptr<DepthStencil> create(const DEPTH_STENCIL_STATE_DESC& desc);
	};
}