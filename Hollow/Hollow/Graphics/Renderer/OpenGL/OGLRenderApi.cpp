#include "OGLRenderApi.h"

namespace Hollow {
	OGLRenderApi::OGLRenderApi(int width, int height)
	{
		HardwareBufferManager::startUp<OGLHardwareBufferManager>();
		TextureManager::startUp<OGLTextureManager>();
		ShaderManager::startUp<OGLShaderManager>();
		GPUBufferManager::startUp<OGLGPUBufferManager>();
		RenderTargetManager::startUp<OGLRenderTargetManager>();

		hwnd = static_cast<OGLWin32Window*>(Window::instance())->getHWND();
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);
	}

	OGLRenderApi::~OGLRenderApi()
	{
		HardwareBufferManager::shutdown();
		TextureManager::shutdown();
		ShaderManager::shutdown();
		GPUBufferManager::shutdown();
		RenderTargetManager::shutdown();
	}

	void OGLRenderApi::SetIndexBuffer(IndexBuffer* buffer)
	{
		OGLIndexBuffer* iBuffer = static_cast<OGLIndexBuffer*>(buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer->IBO);
	}

	void OGLRenderApi::SetVertexBuffer(VertexBuffer* buffer)
	{
		glBindVertexArray(static_cast<OGLVertexBuffer*>(buffer)->VAO);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);
	}

	void OGLRenderApi::SetTexture(UINT location, Texture* texture)
	{
		OGLTexture* oglTexture = static_cast<OGLTexture*>(texture);
		glActiveTexture(location + GL_TEXTURE0);
		if (texture->type == TextureType::TEXTURE2D) {
			glBindTexture(GL_TEXTURE_2D, oglTexture->textureId);
		} else {
			glBindTexture(GL_TEXTURE_CUBE_MAP, oglTexture->textureId);
		}
	}

	void OGLRenderApi::UnsetTexture(UINT slot)
	{
		glActiveTexture(slot + GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OGLRenderApi::SetTextureColorBuffer(UINT slot, RenderTarget* renderTarget, UINT targetNum)
	{
		OGLRenderTarget* oglRenderTarget = static_cast<OGLRenderTarget*>(renderTarget);
		glActiveTexture(slot + GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, oglRenderTarget->texture[targetNum]);
	}

	void OGLRenderApi::SetTextureDepthBuffer(UINT slot, RenderTarget* renderTarget)
	{
		OGLRenderTarget* oglRenderTarget = static_cast<OGLRenderTarget*>(renderTarget);
		glActiveTexture(slot + GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, oglRenderTarget->depth);
	}

	void OGLRenderApi::SetShader(ShaderProgram* shader)
	{
		glUseProgram(static_cast<OGLShaderProgram*>(shader)->shaderId);
	}

	void OGLRenderApi::SetGpuBuffer(GPUBuffer* buffer)
	{
		OGLGpuBuffer* gpuBuffer = static_cast<OGLGpuBuffer*>(buffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, gpuBuffer->getLocation(), gpuBuffer->UBO);
	}

	void OGLRenderApi::SetViewport(int w0, int y0, int w, int y)
	{
		glViewport(w0, y0, w, y);
	}

	void OGLRenderApi::ClearRenderTarget(RenderTarget* renderTarget, const float* color)
	{
		OGLRenderTarget* oglRenderTarget = static_cast<OGLRenderTarget*>(renderTarget);

		if (oglRenderTarget != nullptr) {
			glBindFramebuffer(GL_FRAMEBUFFER, oglRenderTarget->FBO);
			glClearColor(color[0], color[1], color[2], color[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		} else {
			glClearColor(color[0], color[1], color[2], color[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}

	void OGLRenderApi::SetRenderTarget(RenderTarget* renderTarget)
	{
		if (renderTarget != nullptr) {
			OGLRenderTarget* oglRenderTarget = static_cast<OGLRenderTarget*>(renderTarget);
			glBindFramebuffer(GL_FRAMEBUFFER, oglRenderTarget->FBO);
		} else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void OGLRenderApi::SetDepthTestFunction(DEPTH_TEST_FUNCTION func)
	{
		switch (func) {
		case DEPTH_TEST_FUNCTION::NEVER:
		{
			glDepthFunc(GL_NEVER);
		} break;
		case DEPTH_TEST_FUNCTION::LESS:
		{
			glDepthFunc(GL_LESS);
		} break;
		case DEPTH_TEST_FUNCTION::EQUAL:
		{
			glDepthFunc(GL_EQUAL);
		} break;
		case DEPTH_TEST_FUNCTION::LEQUAL:
		{
			glDepthFunc(GL_LEQUAL);
		} break;
		case DEPTH_TEST_FUNCTION::GREATER:
		{
			glDepthFunc(GL_GREATER);
		} break;
		case DEPTH_TEST_FUNCTION::NOT_EQUAL:
		{
			glDepthFunc(GL_NOTEQUAL);
		} break;
		case DEPTH_TEST_FUNCTION::ALWAYS:
		{
			glDepthFunc(GL_ALWAYS);
		} break;
		}
	}

	void OGLRenderApi::SetCullMode(CULL_MODE mode)
	{
		switch (mode)
		{
		case Hollow::CULL_NONE:
			glDisable(GL_CULL_FACE);
			cullEnabled = false;
			break;
		case Hollow::CULL_FRONT:
			if (!cullEnabled) {
				glEnable(GL_CULL_FACE);
				cullEnabled = true;
			}
			glCullFace(GL_FRONT);
			break;
		case Hollow::CULL_BACK:
			if (!cullEnabled) {
				glEnable(GL_CULL_FACE);
				cullEnabled = true;
			}
			glCullFace(GL_BACK);
			break;
		}
	}

	void OGLRenderApi::DrawIndexed(UINT count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	}

	void OGLRenderApi::Present()
	{
		SwapBuffers(GetDC(*hwnd));
	}
}