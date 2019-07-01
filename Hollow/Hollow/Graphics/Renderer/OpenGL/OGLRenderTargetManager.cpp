#include "OGLRenderTargetManager.h"

namespace Hollow {
	RenderTarget* OGLRenderTargetManager::create(int width, int height, RENDER_TARGET_DESC desc)
	{
		OGLRenderTarget* renderTarget = new OGLRenderTarget(width, height, desc.count);

		renderTarget->texture = new unsigned int[desc.count];

		glGenFramebuffers(1, &renderTarget->FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->FBO);

		glGenTextures(desc.count, renderTarget->texture);
		for (int i = 0; i < desc.count; i++) {
			glBindTexture(GL_TEXTURE_2D, renderTarget->texture[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, renderTarget->texture[i], 0);
		}

		glGenTextures(1, &renderTarget->depth);
		glBindTexture(GL_TEXTURE_2D, renderTarget->depth);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, renderTarget->depth, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			HW_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return renderTarget;
	}
}