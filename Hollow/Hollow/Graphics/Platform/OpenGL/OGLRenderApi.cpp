#include "OGLRenderApi.h"

namespace Hollow {
	OGLRenderApi::OGLRenderApi(int width, int height)
	{
		HardwareBufferManager::startUp<OGLHardwareBufferManager>();
		TextureManager::startUp<OGLTextureManager>();
		ShaderManager::startUp<OGLShaderManager>();
		GPUBufferManager::startUp<OGLGPUBufferManager>();
		RenderTargetManager::startUp<OGLRenderTargetManager>();
		InputLayoutManager::startUp<OGLInputLayoutManager>();
		RenderStateManager::startUp<OGLRenderStateManager>();

		hwnd = static_cast<OGLWin32Window*>(Window::instance())->getHWND();
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CW);
	}

	OGLRenderApi::~OGLRenderApi()
	{
	}

	void OGLRenderApi::onShutdown()
	{
		RenderStateManager::shutdown();
		InputLayoutManager::shutdown();
		RenderTargetManager::shutdown();
		GPUBufferManager::shutdown();
		ShaderManager::shutdown();
		TextureManager::shutdown();
		HardwareBufferManager::shutdown();
	}

	void OGLRenderApi::setIndexBuffer(const s_ptr<IndexBuffer>& buffer)
	{
		s_ptr<OGLIndexBuffer> iBuffer = std::static_pointer_cast<OGLIndexBuffer>(buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, std::static_pointer_cast<OGLHardwareBuffer>(iBuffer->mHardwareBuffer)->mVbo);
		mCurrentIndexBuffer = iBuffer;
	}

	void OGLRenderApi::setVertexBuffer(const s_ptr<VertexBuffer>& buffer)
	{
		OGLHardwareBuffer* oglBuffer = std::static_pointer_cast<OGLHardwareBuffer>(buffer->mHardwareBuffer).get();
		glBindVertexBuffer(0, oglBuffer->mVbo, 0, buffer->mHardwareBuffer->getStride());
	}

	void OGLRenderApi::setTexture(UINT location, const s_ptr<Texture>& texture)
	{
		OGLTexture* oglTexture = std::static_pointer_cast<OGLTexture>(texture).get();

		glActiveTexture(location + GL_TEXTURE0);
		if (texture->type == TextureType::TT_TEXTURE2D) 
		{
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

	void OGLRenderApi::setTextureColorBuffer(UINT slot, const s_ptr<RenderTarget>& renderTarget, UINT targetNum)
	{
		s_ptr<OGLRenderTarget> oglRenderTarget = std::static_pointer_cast<OGLRenderTarget>(renderTarget);
		glActiveTexture(slot + GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, oglRenderTarget->texture[targetNum]);
	}

	void OGLRenderApi::setTextureDepthBuffer(UINT slot, const s_ptr<RenderTarget>& renderTarget)
	{
		s_ptr<OGLRenderTarget> oglRenderTarget = std::static_pointer_cast<OGLRenderTarget>(renderTarget);
		glActiveTexture(slot + GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, oglRenderTarget->depth);
	}

	void OGLRenderApi::setGpuBuffer(const s_ptr<GPUBuffer>& buffer)
	{
		s_ptr<OGLGpuBuffer> gpuBuffer = std::static_pointer_cast<OGLGpuBuffer>(buffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, gpuBuffer->getLocation(), gpuBuffer->UBO);
	}

	void OGLRenderApi::setViewport(int w0, int y0, int w, int y)
	{
		glViewport(w0, y0, w, y);
	}

	void OGLRenderApi::clearRenderTarget(const s_ptr<RenderTarget>& renderTarget, const float* color)
	{
		s_ptr<OGLRenderTarget> oglRenderTarget = std::static_pointer_cast<OGLRenderTarget>(renderTarget);

		if (oglRenderTarget != nullptr) 
		{
			glBindFramebuffer(GL_FRAMEBUFFER, oglRenderTarget->FBO);
			glClearColor(color[0], color[1], color[2], color[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		} else {
			glClearColor(color[0], color[1], color[2], color[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}
	}

	void OGLRenderApi::setRenderTarget(const s_ptr<RenderTarget>& renderTarget)
	{
		if (renderTarget != nullptr) 
		{
			s_ptr<OGLRenderTarget> oglRenderTarget = std::static_pointer_cast<OGLRenderTarget>(renderTarget);
			glBindFramebuffer(GL_FRAMEBUFFER, oglRenderTarget->FBO);
		} else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void OGLRenderApi::setInputLayout(const s_ptr<InputLayout>& layout)
	{
		mCurrentLayout = layout;
		s_ptr<OGLInputLayout> oglLayout = std::static_pointer_cast<OGLInputLayout>(layout);
		glBindVertexArray(oglLayout->vao);
	}

	void OGLRenderApi::setDepthStencilState(const s_ptr<DepthStencil>& depthStencil)
	{
		const DEPTH_STENCIL_STATE_DESC& desc = std::static_pointer_cast<OGLDepthStencilState>(depthStencil)->desc;

		if (desc.depthEnable) {
			if (!depthEnabled) {
				depthEnabled = true;
				glEnable(GL_DEPTH_TEST);
			}
			glDepthFunc(OGLHelper::getComparisonFunction(desc.depthFunc));
		} else if (depthEnabled) {
			depthEnabled = false;
			glDisable(GL_DEPTH_TEST);
		}

		if (desc.stencilEnable) {
			if (!stencilEnabled) {
				stencilEnabled = true;
				glEnable(GL_STENCIL_TEST);
			}

			glStencilMask(desc.stencilWriteMask);

			glStencilFuncSeparate(GL_FRONT, OGLHelper::getComparisonFunction(desc.front.stencilFunc), 0xFF, desc.stencilReadMask);
			glStencilOpSeparate(GL_FRONT, OGLHelper::getDepthStencilOperation(desc.front.failOp), OGLHelper::getDepthStencilOperation(desc.front.depthFailOp), OGLHelper::getDepthStencilOperation(desc.front.passOp));

			glStencilFuncSeparate(GL_BACK, OGLHelper::getComparisonFunction(desc.back.stencilFunc), 0xFF, desc.stencilReadMask);
			glStencilOpSeparate(GL_BACK, OGLHelper::getDepthStencilOperation(desc.back.failOp), OGLHelper::getDepthStencilOperation(desc.back.depthFailOp), OGLHelper::getDepthStencilOperation(desc.back.passOp));
		} else if (stencilEnabled) {
			glDisable(GL_STENCIL_TEST);
			stencilEnabled = false;
		}
	}

	void OGLRenderApi::setSampler(const int samplerUnit, const s_ptr<SamplerState>& sampler)
	{

	}

	void OGLRenderApi::setTextureSampler(const int textureUnit, const s_ptr<SamplerState>& sampler)
	{
		glBindSampler(textureUnit, std::static_pointer_cast<OGLSamplerState>(sampler)->samplerObjectId);
	}

	void OGLRenderApi::setRasterizerState(const s_ptr<RasterizerState>& rasterizerState)
	{
		const RASTERIZER_STATE_DESC& desc = std::static_pointer_cast<OGLRasterizerState>(rasterizerState)->desc;

		if (desc.frontCounterClockwise != frontCounterClockwise) {
			frontCounterClockwise = desc.frontCounterClockwise;
			if (desc.frontCounterClockwise) {
				glFrontFace(GL_CCW);
			} else {
				glFrontFace(GL_CW);
			}
		}
		

		switch (desc.cullMode)
		{
		case CullMode::CLM_BACK:
			if (!cullEnabled) {
				cullEnabled = true;
				glEnable(GL_CULL_FACE);
			}
			if (cullMode != CullMode::CLM_BACK) {
				glCullFace(GL_BACK);
			}
			break;
		case CullMode::CLM_FRONT:
			if (!cullEnabled) {
				cullEnabled = true;
				glEnable(GL_CULL_FACE);
			}
			if (cullMode != CullMode::CLM_FRONT) {
				glCullFace(GL_FRONT);
			}
			break;
		case CullMode::CLM_NONE:
			if (cullEnabled) {
				cullEnabled = false;
				glDisable(GL_CULL_FACE);
			}
			break;
		}

		switch (desc.fillMode)
		{
		case FillMode::FM_SOLID:
			if (fillMode != FillMode::FM_SOLID) {
				fillMode = FillMode::FM_SOLID;

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			break;
		case FillMode::FM_WIREFRAME:
			if (fillMode != FillMode::FM_WIREFRAME) {
				fillMode = FillMode::FM_WIREFRAME;

				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			break;
		}
	}

	void OGLRenderApi::setBlendState(const s_ptr<BlendState>& blendState)
	{
		const BLEND_STATE_DESC& desc = std::static_pointer_cast<OGLBlendState>(blendState)->desc;

		for (int i = 0; i < 8; i++) {
			if (desc.blend[i].blendEnabled) {
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
			} else {
				glDisablei(GL_BLEND, i);
			}
		}
	}

	void OGLRenderApi::setShaderPipeline(const s_ptr<ShaderPipeline>& shaderPipeline)
	{
		s_ptr<OGLShaderPipeline> oglPipeline = std::static_pointer_cast<OGLShaderPipeline>(shaderPipeline);
		glBindProgramPipeline(oglPipeline->pipelineId);
	}

	void OGLRenderApi::setPrimitiveTopology(const PrimitiveTopology topology)
	{
		if (topology == mTopology) return;
		mTopology = topology;
		mOglCurrentTopology = OGLHelper::getTopology(mTopology);
	}

	void OGLRenderApi::drawInstanced(UINT count, UINT instanceCount)
	{
		glDrawArraysInstanced(mOglCurrentTopology, 0, count, instanceCount);
	}

	void OGLRenderApi::drawIndexedInstanced(UINT count, UINT instanceCount)
	{
		glDrawElementsInstanced(mOglCurrentTopology, count, GL_UNSIGNED_INT, 0, instanceCount);
	}

	void OGLRenderApi::draw(UINT count)
	{
		glDrawArrays(mOglCurrentTopology, 0, count);
	}

	void OGLRenderApi::drawIndexed(UINT count)
	{
		glDrawElements(mOglCurrentTopology, count, GL_UNSIGNED_INT, 0);
	}

	void OGLRenderApi::present()
	{
		SwapBuffers(GetDC(hwnd));
	}
}