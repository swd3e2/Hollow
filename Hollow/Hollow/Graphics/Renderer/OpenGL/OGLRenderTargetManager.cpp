#include "OGLRenderTargetManager.h"

namespace Hollow {
	RenderTarget* OGLRenderTargetManager::create(int width, int height, RenderTargetFlags flags)
	{
		OGLRenderTarget* renderTarget = new OGLRenderTarget(width, height);

		glGenFramebuffers(1, &renderTarget->FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->FBO);

		glGenTextures(1, &renderTarget->texture);
		glBindTexture(GL_TEXTURE_2D, renderTarget->texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTarget->texture, 0);

		glGenTextures(1, &renderTarget->depth);
		glBindTexture(GL_TEXTURE_2D, renderTarget->depth);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderTarget->depth, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			HW_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return renderTarget;
	}
}