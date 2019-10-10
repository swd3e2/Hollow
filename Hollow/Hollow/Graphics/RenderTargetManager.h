#pragma once

#ifndef HW_RENDER_TARGET_MANAGER_H
#define HW_RENDER_TARGET_MANAGER_H

#include "RenderTarget.h"
#include "Hollow/Core/CModule.h"

namespace Hollow {
	class RenderTargetManager : public CModule<RenderTargetManager>
	{
	public:
		virtual s_ptr<RenderTarget> create(RENDER_TARGET_DESC desc) = 0;
	};
}

#endif