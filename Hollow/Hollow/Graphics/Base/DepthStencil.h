#pragma once

#include "CommonTypes.h"
#include <stdint.h>
#include "CommonTypes.h"

namespace Hollow {
	struct DEPTH_STENCIL_STATEOP_DESC
	{
		StencilOperation failOp;
		StencilOperation depthFailOp;
		StencilOperation passOp;
		ComparisonFunction stencilFunc;

		DEPTH_STENCIL_STATEOP_DESC() :
			failOp(StencilOperation::SOP_KEEP), depthFailOp(StencilOperation::SOP_KEEP), passOp(StencilOperation::SOP_KEEP), 
			stencilFunc(ComparisonFunction::CMP_EQUAL)
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
			depthEnable(true), depthFunc(ComparisonFunction::CMP_LEQUAL), depthWriteMask(DepthWriteMask::DWM_ALL), 
			stencilEnable(false), stencilWriteMask(0xFF), stencilReadMask(0xFF)
		{}
	};

	class DepthStencil
	{
	public:
		static DepthStencil* create(const DEPTH_STENCIL_STATE_DESC& desc);
	};
}