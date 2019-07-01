#pragma once

#ifndef HW_RENDER_TARGET_MANAGER_H
#define HW_RENDER_TARGET_MANAGER_H

#include "Renderer/Base/RenderTarget.h"
#include "Hollow/Core/CModule.h"

namespace Hollow {
	class RenderTargetManager : public CModule<RenderTargetManager>
	{
	public:
		virtual RenderTarget* create(int width, int height, RENDER_TARGET_DESC desc) = 0;
	};
}

#endif