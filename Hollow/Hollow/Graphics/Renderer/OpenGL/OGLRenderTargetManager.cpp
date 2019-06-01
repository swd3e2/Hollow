#include "OGLRenderTargetManager.h"

RenderTarget* OGLRenderTargetManager::create(int width, int height)
{
	OGLRenderTarget* renderTarget = new OGLRenderTarget(width, height);

	glGenFramebuffers(1, &renderTarget->FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->FBO);

	glGenTextures(1, &renderTarget->texture);
	glBindTexture(GL_TEXTURE_2D, renderTarget->texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTarget->texture, 0);

	glGenTextures(1, &renderTarget->depth);
	glBindTexture(GL_TEXTURE_2D, renderTarget->depth);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, renderTarget->depth, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		HW_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return renderTarget;
}
