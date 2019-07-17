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

	void OGLRenderApi::SetIndexBuffer(IndexBuffer* buffer)
	{
		OGLIndexBuffer* iBuffer = static_cast<OGLIndexBuffer*>(buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<OGLHardwareBuffer*>(iBuffer->mHardwareBuffer)->mVertexArrayObject);
	}

	void OGLRenderApi::SetVertexBuffer(VertexBuffer* buffer)
	{
		OGLHardwareBuffer* oglBuffer = static_cast<OGLHardwareBuffer*>(buffer->mHardwareBuffer);
		glBindVertexArray(oglBuffer->mVertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, oglBuffer->mVertexArrayBuffer);
		//glBindVertexBuffer(0, oglBuffer->mVertexArrayBuffer, 0, oglBuffer->getStride());

		//if (mCurrentLayout != nullptr && oglBuffer->mCurrentInputLayout != mCurrentLayout) {
		//	oglBuffer->mCurrentInputLayout = mCurrentLayout;

		//	for (int i = 0; i < mCurrentLayout->layout.size(); i++) {
		//		glEnableVertexAttribArray(i);
		//		GLuint format = OGLHelper::getInputLayoutFormat(mCurrentLayout->layout[i].type);
		//		if (format == GL_FLOAT) {
		//			glVertexAttribFormat(i, mCurrentLayout->layout[i].getNumberElements(), OGLHelper::getInputLayoutFormat(mCurrentLayout->layout[i].type), GL_FALSE, mCurrentLayout->layout[i].offset);
		//		} else if (format == GL_INT) {
		//			glVertexAttribIFormat(i, mCurrentLayout->layout[i].getNumberElements(), OGLHelper::getInputLayoutFormat(mCurrentLayout->layout[i].type), mCurrentLayout->layout[i].offset);
		//		}
		//		/*
		//			glVertexAttribPointer(POSITION_LOCATION,	3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	
		//			glVertexAttribPointer(TEX_COORD_LOCATION,	2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);	
		//			glVertexAttribPointer(NORMAL_LOCATION,		3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);	
		//			glVertexAttribPointer(TANGENT_LOCATION,		3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);	
		//			glVertexAttribPointer(BITANGENT_LOCATION,	3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)44);	
		//			glVertexAttribIPointer(BONE_ID_LOCATION,	4, GL_INT,	sizeof(Vertex), (const GLvoid*)56);	
		//			glVertexAttribPointer(WEIGHTS_LOCATION,		4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)72);
		//		*/
		//		glVertexAttribBinding(i, 0);
		//	}
		//}
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

	void OGLRenderApi::SetLayout(InputLayout* layout)
	{
		mCurrentLayout = layout;
	}

	void OGLRenderApi::SetPipelineState(PipelineState* pipeline)
	{
		OGLPipelineState* oglPipeline = static_cast<OGLPipelineState*>(pipeline);
		glBindProgramPipeline(oglPipeline->pipelineId);
	}

	void OGLRenderApi::DrawIndexed(UINT count)
	{
		GLenum err = glGetError();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	}

	void OGLRenderApi::Present()
	{
		SwapBuffers(GetDC(*hwnd));
	}
}