#include "OGLRenderTargetManager.h"

namespace Hollow {
	RenderTarget* OGLRenderTargetManager::create(int width, int height, RENDER_TARGET_DESC desc)
	{
		OGLRenderTarget* renderTarget = new OGLRenderTarget(width, height, desc.count);

		renderTarget->texture = new unsigned int[desc.count];

		unsigned int* temp = new unsigned int[desc.count];

		glGenFramebuffers(1, &renderTarget->FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->FBO);

		glGenTextures(desc.count, renderTarget->texture);

		for (int i = 0; i < desc.count; i++) {
			glBindTexture(GL_TEXTURE_2D, renderTarget->texture[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, GetTextureFormat(desc.textureFormat), width, height, 0, GL_RGBA, GetTextureType(desc.textureFormat), NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			glBindTexture(GL_TEXTURE_2D, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, renderTarget->texture[i], 0);
			temp[i] = GL_COLOR_ATTACHMENT0 + i;
		}

		glDrawBuffers(desc.count, temp);

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

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return renderTarget;
	}

	unsigned int OGLRenderTargetManager::GetTextureFormat(RENDER_TARGET_TEXTURE_FORMAT format)
	{
		switch (format)
		{
			case R8G8B8A8: {
				return GL_RGBA;
			} break;
			case R32G32B32A32: {
				return GL_RGBA32F;
			} break;
			default:
				return GL_RGBA;
		}
	}
	unsigned int OGLRenderTargetManager::GetTextureType(RENDER_TARGET_TEXTURE_FORMAT format)
	{
		switch (format)
		{
			case R8G8B8A8: {
				return GL_UNSIGNED_BYTE;
			} break;
			case R32G32B32A32: {
				return GL_FLOAT;
			} break;
			default:
				return GL_UNSIGNED_BYTE;
		}
	}
}