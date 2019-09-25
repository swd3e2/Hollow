#pragma once

#ifndef HW_OGL_RENDER_API_H
#define HW_OGL_RENDER_API_H

#include "OGLPrerequisites.h"
#include "Hollow/Graphics/Base/RenderApi.h"
#include <iostream>
#include "Hollow/Common/Log.h"
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Graphics/Camera.h"
#include "OGLHardwareBufferManager.h"
#include "OGLTextureManager.h"
#include "OGLShaderManager.h"
#include "OGLGPUBufferManager.h"
#include "OGLRenderTargetManager.h"
#include "Hollow/Graphics/Base/Window.h"
#include "OGLInputLayout.h"
#include "OGLHelper.h"
#include "OGLPipelineStateManager.h"
#include "OGLInputLayoutManager.h"
#include "OGLBlendState.h"
#include "OGLDepthStencilState.h"
#include "OGLRasterizerState.h"
#include "OGLRenderStateManager.h"

namespace Hollow {
	class OGLRenderApi : public RenderApi
	{
	private:
		HWND hwnd;
		bool cullEnabled = false;
		bool depthEnabled = false;
		bool stencilEnabled = false;
		FillMode fillMode;
		CullMode cullMode;
		OGLIndexBuffer* mCurrentIndexBuffer;
	public:
		OGLRenderApi(int width, int height);
		~OGLRenderApi();
		virtual void setIndexBuffer(IndexBuffer*) override;
		virtual void setVertexBuffer(VertexBuffer*) override;
		virtual void setTexture(UINT, Texture*) override;
		virtual void unsetTexture(UINT) override;
		virtual void setTextureColorBuffer(UINT, RenderTarget*, UINT targetNum) override;
		virtual void setTextureDepthBuffer(UINT, RenderTarget*) override;
		virtual void setGpuBuffer(GPUBuffer*) override;
		virtual void setViewport(int w0, int y0, int w, int y) override;
		virtual void clearRenderTarget(RenderTarget* renderTarget, const float* color) override;
		virtual void setRenderTarget(RenderTarget* renderTarget) override;
		virtual void setInputLayout(InputLayout* layout) override;
		virtual void setPipelineState(PipelineState* pipeline) override;
		virtual void draw(UINT count) override;
		virtual void drawIndexed(UINT count) override;
		virtual void present() override;
		
		virtual void setSampler(const int samplerUnit, SamplerState* sampler) override;
		virtual void setTextureSampler(const int textureUnit, SamplerState* sampler) override;
		virtual void setDepthStencilState(DepthStencil* depthStencil) override;
		virtual void setRasterizerState(RasterizerState* rasterizerState) override;
		virtual void setBlendState(BlendState* blendState) override;
		virtual void setShaderPipeline(ShaderPipeline* shaderPipeline) override;

		virtual void drawInstanced() override;
		virtual void drawIndexedInstanced() override;
	};
}

#endif