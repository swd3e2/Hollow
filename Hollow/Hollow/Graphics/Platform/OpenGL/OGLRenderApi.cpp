#include "OGLRenderApi.h"

namespace Hollow {
	OGLRenderApi::OGLRenderApi(int width, int height)
	{
		HardwareBufferManager::startUp<OGLHardwareBufferManager>();
		TextureManager::startUp<OGLTextureManager>();
		ShaderManager::startUp<OGLShaderManager>();
		GPUBufferManager::startUp<OGLGPUBufferManager>();
		RenderTargetManager::startUp<OGLRenderTargetManager>();
		PipelineStateManager::startUp<OGLPipelineStateManager>();
		InputLayoutManager::startUp<OGLInputLayoutManager>();

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
		PipelineStateManager::shutdown();
		InputLayoutManager::shutdown();
	}

	void OGLRenderApi::setIndexBuffer(IndexBuffer* buffer)
	{
		OGLIndexBuffer* iBuffer = static_cast<OGLIndexBuffer*>(buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<OGLHardwareBuffer*>(iBuffer->mHardwareBuffer)->mVbo);
		mCurrentIndexBuffer = iBuffer;
	}

	void OGLRenderApi::setVertexBuffer(VertexBuffer* buffer)
	{
		OGLHardwareBuffer* oglBuffer = static_cast<OGLHardwareBuffer*>(buffer->mHardwareBuffer);
		glBindVertexBuffer(0, oglBuffer->mVbo, 0, static_cast<OGLInputLayout*>(mCurrentLayout)->size);
	}

	void OGLRenderApi::setTexture(UINT location, Texture* texture)
	{
		OGLTexture* oglTexture = static_cast<OGLTexture*>(texture);
		glActiveTexture(location + GL_TEXTURE0);
		if (texture->type == TextureType::TEXTURE2D) {
			glBindTexture(GL_TEXTURE_2D, oglTexture->textureId);
		} else {
			glBindTexture(GL_TEXTURE_CUBE_MAP, oglTexture->textureId);
		}
	}

	void OGLRenderApi::unsetTexture(UINT slot)
	{
		glActiveTexture(slot + GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OGLRenderApi::setTextureColorBuffer(UINT slot, RenderTarget* renderTarget, UINT targetNum)
	{
		OGLRenderTarget* oglRenderTarget = static_cast<OGLRenderTarget*>(renderTarget);
		glActiveTexture(slot + GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, oglRenderTarget->texture[targetNum]);
	}

	void OGLRenderApi::setTextureDepthBuffer(UINT slot, RenderTarget* renderTarget)
	{
		OGLRenderTarget* oglRenderTarget = static_cast<OGLRenderTarget*>(renderTarget);
		glActiveTexture(slot + GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, oglRenderTarget->depth);
	}

	void OGLRenderApi::setGpuBuffer(GPUBuffer* buffer)
	{
		OGLGpuBuffer* gpuBuffer = static_cast<OGLGpuBuffer*>(buffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, gpuBuffer->getLocation(), gpuBuffer->UBO);
	}

	void OGLRenderApi::setViewport(int w0, int y0, int w, int y)
	{
		glViewport(w0, y0, w, y);
	}

	void OGLRenderApi::clearRenderTarget(RenderTarget* renderTarget, const float* color)
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

	void OGLRenderApi::setRenderTarget(RenderTarget* renderTarget)
	{
		if (renderTarget != nullptr) {
			OGLRenderTarget* oglRenderTarget = static_cast<OGLRenderTarget*>(renderTarget);
			glBindFramebuffer(GL_FRAMEBUFFER, oglRenderTarget->FBO);
		} else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void OGLRenderApi::setDepthTestFunction(DEPTH_TEST_FUNCTION func)
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

	void OGLRenderApi::setCullMode(CULL_MODE mode)
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

	void OGLRenderApi::setInputLayout(InputLayout* layout)
	{
		mCurrentLayout = layout;
		OGLInputLayout* oglLayout = static_cast<OGLInputLayout*>(layout);
		glBindVertexArray(oglLayout->vao);
	}

	void OGLRenderApi::setPipelineState(PipelineState* pipeline)
	{
		OGLPipelineState* oglPipeline = static_cast<OGLPipelineState*>(pipeline);
		glBindProgramPipeline(oglPipeline->pipelineId);
	}

	void OGLRenderApi::drawIndexed(UINT count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	}

	void OGLRenderApi::present()
	{
		SwapBuffers(GetDC(*hwnd));
	}
}