#pragma once

#ifndef HW_OGL_RENDER_API_H
#define HW_OGL_RENDER_API_H

#include "OGLPrerequisites.h"
#include "Hollow/Graphics/RenderApi.h"
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Graphics/Camera.h"
#include "OGLHardwareBufferManager.h"
#include "OGLTextureManager.h"
#include "OGLShaderManager.h"
#include "OGLGPUBufferManager.h"
#include "OGLRenderTargetManager.h"
#include "Hollow/Graphics/Window.h"
#include "OGLInputLayout.h"
#include "OGLHelper.h"
#include "OGLShaderPipeline.h"
#include "OGLInputLayoutManager.h"
#include "OGLBlendState.h"
#include "OGLDepthStencilState.h"
#include "OGLRasterizerState.h"
#include "OGLRenderStateManager.h"
#include "Win32/OGLWin32Window.h"

namespace Hollow {
	class OGLRenderApi : public RenderApi
	{
	private:
		HWND hwnd;
		bool cullEnabled = false;
		bool depthEnabled = false;
		bool stencilEnabled = false;
		bool frontCounterClockwise = false;
		FillMode fillMode;
		CullMode cullMode;
		s_ptr<OGLIndexBuffer> mCurrentIndexBuffer;
		GLuint mOglCurrentTopology;
	public:
		OGLRenderApi(int width, int height);
		~OGLRenderApi();

		virtual void setIndexBuffer(const s_ptr<IndexBuffer>& indexBuffer) override;
		virtual void setVertexBuffer(const s_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void setTexture(UINT, const s_ptr<Texture>& texture) override;
		virtual void unsetTexture(UINT) override;
		virtual void setTextureColorBuffer(UINT, const s_ptr<RenderTarget>& renderTarget, UINT targetNum) override;
		virtual void setTextureDepthBuffer(UINT, const s_ptr<RenderTarget>& renderTarget) override;
		virtual void setGpuBuffer(const s_ptr<GPUBuffer>& gBuffer) override;
		virtual void setViewport(int w0, int y0, int w, int y) override;
		virtual void clearRenderTarget(const s_ptr<RenderTarget>& renderTarget, const float* color) override;
		virtual void setRenderTarget(const s_ptr<RenderTarget>& renderTarget) override;
		virtual void setInputLayout(const s_ptr<InputLayout>& layout) override;
		virtual void draw(UINT count) override;
		virtual void drawIndexed(UINT count) override;
		virtual void present() override;
		
		virtual void setSampler(const int samplerUnit, const s_ptr<SamplerState>& sampler) override;
		virtual void setTextureSampler(const int textureUnit, const s_ptr<SamplerState>& sampler) override;
		virtual void setDepthStencilState(const s_ptr<DepthStencil>& depthStencil) override;
		virtual void setRasterizerState(const s_ptr<RasterizerState>& rasterizerState) override;
		virtual void setBlendState(const s_ptr<BlendState>& blendState) override;
		virtual void setShaderPipeline(const s_ptr<ShaderPipeline>& shaderPipeline) override;
		virtual void setPrimitiveTopology(const PrimitiveTopology topology) override;
		virtual void drawInstanced(UINT count, UINT instanceCount) override;
		virtual void drawIndexedInstanced(UINT count, UINT instanceCount) override;

		virtual RendererType getRendererType() override { return RendererType::OpenGL; }
	};
}

#endif