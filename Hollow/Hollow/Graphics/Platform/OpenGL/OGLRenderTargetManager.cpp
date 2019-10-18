#include "OGLRenderTargetManager.h"

namespace Hollow {
	s_ptr<RenderTarget> OGLRenderTargetManager::create(RENDER_TARGET_DESC desc)
	{
		OGLRenderTarget* renderTarget = new OGLRenderTarget(desc.width, desc.height, desc.count);

		renderTarget->texture = new unsigned int[desc.count];

		unsigned int* temp = new unsigned int[desc.count];

		glCreateFramebuffers(1, &renderTarget->FBO);
		glCreateTextures(GL_TEXTURE_2D, desc.count, renderTarget->texture);

		for (int i = 0; i < desc.count; i++) {
			glTextureStorage2D(renderTarget->texture[i], 1, getTextureFormat(desc.textureFormat), desc.width, desc.height);
			glNamedFramebufferTexture(renderTarget->FBO, GL_COLOR_ATTACHMENT0 + i, renderTarget->texture[i], 0);
			temp[i] = GL_COLOR_ATTACHMENT0 + i;
		}

		glNamedFramebufferDrawBuffers(renderTarget->FBO, desc.count, temp);

		glCreateTextures(GL_TEXTURE_2D, 1, &renderTarget->depth);
		glTextureStorage2D(renderTarget->depth, 1, GL_DEPTH24_STENCIL8, desc.width, desc.height);

		glNamedFramebufferTexture(renderTarget->FBO, GL_DEPTH_STENCIL_ATTACHMENT, renderTarget->depth, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, renderTarget->FBO);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			HW_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		}

		return s_ptr<RenderTarget>(renderTarget);
	}

	unsigned int OGLRenderTargetManager::getTextureFormat(RENDER_TARGET_TEXTURE_FORMAT format)
	{
		switch (format)
		{
			case RENDER_TARGET_TEXTURE_FORMAT::R8G8B8A8:
				return GL_RGBA8;
			case RENDER_TARGET_TEXTURE_FORMAT::R32G32B32A32:
				return GL_RGBA32F;
			default:
				return GL_RGBA;
		}
	}
	unsigned int OGLRenderTargetManager::getTextureType(RENDER_TARGET_TEXTURE_FORMAT format)
	{
		switch (format)
		{
		case RENDER_TARGET_TEXTURE_FORMAT::R8G8B8A8:
				return GL_UNSIGNED_BYTE;
			case RENDER_TARGET_TEXTURE_FORMAT::R32G32B32A32:
				return GL_FLOAT;
			default:
				return GL_UNSIGNED_BYTE;
		}
	}
}