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
		RenderStateManager::startUp<OGLRenderStateManager>();

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
		glBindVertexBuffer(0, oglBuffer->mVbo, 0, buffer->mHardwareBuffer->getStride());
	}

	void OGLRenderApi::setTexture(UINT location, Texture* texture)
	{
		OGLTexture* oglTexture = static_cast<OGLTexture*>(texture);
		glActiveTexture(location + GL_TEXTURE0);
		if (texture->type == TextureType::TEXTURE2D) 
		{
			glBindTexture(GL_TEXTURE_2D, oglTexture->textureId);
		} 
		else 
		{
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

		if (oglRenderTarget != nullptr) 
		{
			glBindFramebuffer(GL_FRAMEBUFFER, oglRenderTarget->FBO);
			glClearColor(color[0], color[1], color[2], color[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		} 
		else 
		{
			glClearColor(color[0], color[1], color[2], color[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}
	}

	void OGLRenderApi::setRenderTarget(RenderTarget* renderTarget)
	{
		if (renderTarget != nullptr) 
		{
			OGLRenderTarget* oglRenderTarget = static_cast<OGLRenderTarget*>(renderTarget);
			glBindFramebuffer(GL_FRAMEBUFFER, oglRenderTarget->FBO);
		} 
		else 
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	void OGLRenderApi::setDepthStencilState(DepthStencil* depthStencil)
	{
		const DEPTH_STENCIL_STATE_DESC& desc = static_cast<OGLDepthStencilState*>(depthStencil)->desc;

		if (desc.depthEnable) 
		{
			if (!depthEnabled) 
			{
				depthEnabled = true;
				glEnable(GL_DEPTH_TEST);
			}
			glDepthFunc(OGLHelper::getComparisonFunction(desc.depthFunc));
		} 
		else if (depthEnabled) 
		{
			depthEnabled = false;
			glDisable(GL_DEPTH_TEST);
		}

		if (desc.stencilEnable) {
			if (!stencilEnabled)
			{
				stencilEnabled = true;
				glEnable(GL_STENCIL_TEST);
			}

			glStencilMask(desc.stencilWriteMask);

			glStencilFuncSeparate(GL_FRONT, OGLHelper::getComparisonFunction(desc.front.stencilFunc), 0xFF, desc.stencilReadMask);
			glStencilOpSeparate(GL_FRONT, OGLHelper::getDepthStencilOperation(desc.front.failOp), OGLHelper::getDepthStencilOperation(desc.front.depthFailOp), OGLHelper::getDepthStencilOperation(desc.front.passOp));

			glStencilFuncSeparate(GL_BACK, OGLHelper::getComparisonFunction(desc.back.stencilFunc), 0xFF, desc.stencilReadMask);
			glStencilOpSeparate(GL_BACK, OGLHelper::getDepthStencilOperation(desc.back.failOp), OGLHelper::getDepthStencilOperation(desc.back.depthFailOp), OGLHelper::getDepthStencilOperation(desc.back.passOp));
		} 
		else if (stencilEnabled) 
		{
			glDisable(GL_STENCIL_TEST);
			stencilEnabled = false;
		}
	}

	void OGLRenderApi::setSampler(const int samplerUnit, SamplerState* sampler)
	{

	}

	void OGLRenderApi::setTextureSampler(const int textureUnit, SamplerState* sampler)
	{
		glBindSampler(textureUnit, static_cast<OGLSamplerState*>(sampler)->samplerObjectId);
	}

	void OGLRenderApi::setRasterizerState(RasterizerState* rasterizerState)
	{
		const RASTERIZER_STATE_DESC& desc = static_cast<OGLRasterizerState*>(rasterizerState)->desc;

		if (desc.frontCounterClockwise != frontCounterClockwise) 
		{
			frontCounterClockwise = desc.frontCounterClockwise;
			if (desc.frontCounterClockwise) 
			{
				glFrontFace(GL_CCW);
			}
			else 
			{
				glFrontFace(GL_CW);
			}
		}
		

		switch (desc.cullMode)
		{
		case CullMode::CLM_BACK:
			if (!cullEnabled)
			{
				cullEnabled = true;
				glEnable(GL_CULL_FACE);
			}
			if (cullMode != CullMode::CLM_BACK)
			{
				glCullFace(GL_BACK);
			}
			break;
		case CullMode::CLM_FRONT:
			if (!cullEnabled)
			{
				cullEnabled = true;
				glEnable(GL_CULL_FACE);
			}
			if (cullMode != CullMode::CLM_FRONT)
			{
				glCullFace(GL_FRONT);
			}
			break;
		case CullMode::CLM_NONE:
			if (cullEnabled)
			{
				cullEnabled = false;
				glDisable(GL_CULL_FACE);
			}
			break;
		}

		if (desc.cullMode != CullMode::CLM_NONE)
		{
			switch (desc.fillMode)
			{
			case FillMode::FM_SOLID:
				if (fillMode != FillMode::FM_SOLID) {
					fillMode = FillMode::FM_SOLID;

					glPolygonMode(GL_FRONT, GL_FILL);
					glPolygonMode(GL_BACK, GL_FILL);
				}
				break;
			case FillMode::FM_WIREFRAME:
				if (fillMode != FillMode::FM_WIREFRAME) {
					fillMode = FillMode::FM_WIREFRAME;

					glPolygonMode(GL_FRONT, GL_LINE);
					glPolygonMode(GL_BACK, GL_LINE);
				}
				break;
			}
		}
	}

	void OGLRenderApi::setBlendState(BlendState* blendState)
	{
		const BLEND_STATE_DESC& desc = static_cast<OGLBlendState*>(blendState)->desc;

		for (int i = 0; i < 8; i++) {
			if (desc.blend[i].blendEnabled) 
			{
				glEnablei(GL_BLEND, i);

				glBlendFuncSeparatei(
					i, 
					OGLHelper::getBlend(desc.blend[i].srcBlend), 
					OGLHelper::getBlend(desc.blend[i].dstBlend), 
					OGLHelper::getBlend(desc.blend[i].srcBlendAlpha),
					OGLHelper::getBlend(desc.blend[i].dstBlendAlpha)
				);
				glBlendEquationSeparatei(
					i, 
					OGLHelper::getBlendOperation(desc.blend[i].blendOp), 
					OGLHelper::getBlendOperation(desc.blend[i].blendOpAlpha)
				);
			}
			else
			{
				glDisablei(GL_BLEND, i);
			}
		}
	}

	void OGLRenderApi::setShaderPipeline(ShaderPipeline* shaderPipeline)
	{

	}

	void OGLRenderApi::drawInstanced()
	{

	}

	void OGLRenderApi::drawIndexedInstanced()
	{

	}

	void OGLRenderApi::draw(UINT count)
	{
		glDrawArrays(GL_TRIANGLES, 0, count);
	}

	void OGLRenderApi::drawIndexed(UINT count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	}

	void OGLRenderApi::present()
	{
		SwapBuffers(GetDC(hwnd));
	}
}