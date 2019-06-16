#include "OGLRenderApi.h"

namespace Hollow {
	OGLRenderApi::OGLRenderApi(int width, int height)
	{
		hardwareBufferManager = new OGLHardwareBufferManager();
		textureManager = new OGLTextureManager();
		shaderManager = new OGLShaderManager();
		gpuBufferManager = new OGLGPUBufferManager();
		renderTargetManager = new OGLRenderTargetManager();
		hwnd = static_cast<OGLWin32Window*>(WindowManager::instance()->getWindow())->getHWND();
		glEnable(GL_DEPTH_TEST);
		//glCullFace(GL_BACK);
		glDepthFunc(GL_LEQUAL);
		setStartedUp();
	}

	void OGLRenderApi::startUp()
	{}

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
		}
		else {
			glBindTexture(GL_TEXTURE_CUBE_MAP, oglTexture->textureId);
		}
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
		}
		else {
			glClearColor(color[0], color[1], color[2], color[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}

	void OGLRenderApi::SetRenderTarget(RenderTarget* renderTarget)
	{
		if (renderTarget != nullptr) {
			OGLRenderTarget* oglRenderTarget = static_cast<OGLRenderTarget*>(renderTarget);
			glBindFramebuffer(GL_FRAMEBUFFER, oglRenderTarget->FBO);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
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