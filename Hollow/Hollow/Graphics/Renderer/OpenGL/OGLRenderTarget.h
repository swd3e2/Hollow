#pragma once

#ifndef HW_OGL_RENDER_TARGET_H
#define HW_OGL_RENDER_TARGET_H

#include "Hollow/Graphics/Renderer/Base/RenderTarget.h"
#include "OGLPrerequisites.h"

namespace Hollow {
	class OGLRenderTarget : public RenderTarget
	{
	public:
		unsigned int FBO;
		unsigned int texture;
		unsigned int depth;
	public:
		OGLRenderTarget(int width, int height) :
			RenderTarget(width, height), FBO(0)
		{}
		virtual Vector4 readPixel(int x, int y) override;
	};
}

#endif