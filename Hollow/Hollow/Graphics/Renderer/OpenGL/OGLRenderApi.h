#pragma once

#ifndef HW_OGL_RENDER_API_H
#define HW_OGL_RENDER_API_H

#include "OGLPrerequisites.h"
#include "Hollow/Graphics/Renderer/Base/RenderApi.h"
#include <iostream>
#include "Hollow/Common/Log.h"
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Graphics/Camera.h"
#include "OGLHardwareBufferManager.h"
#include "OGLTextureManager.h"
#include "OGLShaderProgram.h"
#include "OGLShaderManager.h"
#include "OGLGPUBufferManager.h"
#include "OGLRenderTargetManager.h"
#include "Hollow/Graphics/Renderer/Base/Window.h"

namespace Hollow {
	class OGLRenderApi : public RenderApi
	{
	private:
		HWND* hwnd;
	public:
		OGLRenderApi(int width, int height);
		~OGLRenderApi();
		virtual void SetIndexBuffer(IndexBuffer*) override;
		virtual void SetVertexBuffer(VertexBuffer*) override;
		virtual void SetTexture(UINT, Texture*) override;
		virtual void UnsetTexture(UINT) override;
		virtual void SetTextureColorBuffer(UINT, RenderTarget*) override;
		virtual void SetTextureDepthBuffer(UINT, RenderTarget*) override;
		virtual void SetShader(ShaderProgram*) override;
		virtual void SetGpuBuffer(GPUBuffer*) override;
		virtual void SetViewport(int w0, int y0, int w, int y) override;
		virtual void ClearRenderTarget(RenderTarget* renderTarget, const float* color) override;
		virtual void SetRenderTarget(RenderTarget* renderTarget) override;

		virtual void Draw(UINT count)override {}
		virtual void DrawIndexed(UINT count) override;
		virtual void Present() override;
	};
}

#endif