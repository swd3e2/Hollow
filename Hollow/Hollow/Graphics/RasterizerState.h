#pragma once

#include "CommonTypes.h"
#include "Hollow/Platform.h"

namespace Hollow {
	struct RASTERIZER_STATE_DESC
	{
		FillMode	fillMode;
		CullMode	cullMode;
		bool        frontCounterClockwise;

		RASTERIZER_STATE_DESC() :
			fillMode(FillMode::FM_SOLID), cullMode(CullMode::CLM_BACK), frontCounterClockwise(false)
		{}
	};

	class RasterizerState
	{
	public:
		static s_ptr<RasterizerState> create(const RASTERIZER_STATE_DESC& desc);
	};
}