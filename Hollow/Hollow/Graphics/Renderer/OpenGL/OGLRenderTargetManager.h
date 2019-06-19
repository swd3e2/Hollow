#pragma once

#ifndef HW_OGL_RENDER_TARGET_MANAGER_H
#define HW_OGL_RENDER_TARGET_MANAGER_H

#include "Hollow/Graphics/RenderTargetManager.h"
#include "OGLPrerequisites.h"
#include "OGLRenderTarget.h"
#include "Hollow/Common/Log.h"

namespace Hollow {
	class OGLRenderTargetManager : public RenderTargetManager
	{
	public:
		virtual RenderTarget* create(int width, int height, RenderTargetFlags flags = RenderTargetFlags::NONE);
	};
}

#endif