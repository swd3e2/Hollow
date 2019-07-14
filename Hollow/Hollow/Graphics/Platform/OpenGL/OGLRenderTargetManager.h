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
		virtual RenderTarget* create(RENDER_TARGET_DESC desc);
	private:
		unsigned int GetTextureFormat(RENDER_TARGET_TEXTURE_FORMAT format);
		unsigned int GetTextureType(RENDER_TARGET_TEXTURE_FORMAT format);
	};
}

#endif