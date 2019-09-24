#include "OGLRenderTargetManager.h"

namespace Hollow {
	RenderTarget* OGLRenderTargetManager::create(RENDER_TARGET_DESC desc)
	{
		OGLRenderTarget* renderTarget = new OGLRenderTarget(desc.width, desc.height, desc.count);

		renderTarget->texture = new unsigned int[desc.count];

		unsigned int* temp = new unsigned int[desc.count];

		glGenFramebuffers(1, &renderTarget->FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->FBO);

		glGenTextures(desc.count, renderTarget->texture);

		for (int i = 0; i < desc.count; i++) {
			glBindTexture(GL_TEXTURE_2D, renderTarget->texture[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, getTextureFormat(desc.textureFormat), desc.width, desc.height, 0, GL_RGBA, getTextureType(desc.textureFormat), NULL);

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

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, desc.width, desc.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderTarget->depth, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			HW_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return renderTarget;
	}

	unsigned int OGLRenderTargetManager::getTextureFormat(RENDER_TARGET_TEXTURE_FORMAT format)
	{
		switch (format)
		{
			case RENDER_TARGET_TEXTURE_FORMAT::R8G8B8A8: {
				return GL_RGBA;
			} break;
			case RENDER_TARGET_TEXTURE_FORMAT::R32G32B32A32: {
				return GL_RGBA32F;
			} break;
			default:
				return GL_RGBA;
		}
	}
	unsigned int OGLRenderTargetManager::getTextureType(RENDER_TARGET_TEXTURE_FORMAT format)
	{
		switch (format)
		{
		case RENDER_TARGET_TEXTURE_FORMAT::R8G8B8A8: {
				return GL_UNSIGNED_BYTE;
			} break;
			case RENDER_TARGET_TEXTURE_FORMAT::R32G32B32A32: {
				return GL_FLOAT;
			} break;
			default:
				return GL_UNSIGNED_BYTE;
		}
	}
}