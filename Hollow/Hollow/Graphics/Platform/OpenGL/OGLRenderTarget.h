#pragma once

#ifndef HW_OGL_RENDER_TARGET_H
#define HW_OGL_RENDER_TARGET_H

#include "Hollow/Graphics/RenderTarget.h"
#include "OGLPrerequisites.h"

namespace Hollow {
	class OGLRenderTarget : public RenderTarget
	{
	public:
		unsigned int FBO;
		unsigned int* texture;
		unsigned int depth;
	public:
		OGLRenderTarget(int width, int height, int count) :
			RenderTarget(width, height, count), FBO(0)
		{}

		virtual ~OGLRenderTarget()
		{
			glDeleteTextures(getCount(), texture);
			glDeleteTextures(1, &depth);
			glDeleteFramebuffers(1, &FBO);
		}

		virtual Vector4 readPixel(int x, int y) override;
	};
}

#endif