#pragma once

#ifndef HW_RENDER_API_H
#define HW_RENDER_API_H

#include "Hollow/Platform.h"
#include "Hollow/Core/CModule.h"
#include "HardwareBuffer.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "GPUBuffer.h"
#include "RenderTarget.h"
#include "InputLayout.h"
#include "PipelineState.h"


namespace Hollow {
	enum DEPTH_TEST_FUNCTION {
		NEVER = 0,
		LESS = 1,
		EQUAL = 2,
		LEQUAL = 3,
		GREATER = 4,
		NOT_EQUAL = 5,
		ALWAYS = 6
	};

	enum CULL_MODE {
		CULL_NONE = 0,
		CULL_FRONT = 1,
		CULL_BACK = 2
	};

	class RenderApi : public CModule<RenderApi>
	{
	protected:
		InputLayout* mCurrentLayout;
	public:
		virtual void SetIndexBuffer(IndexBuffer*) = 0;
		virtual void SetVertexBuffer(VertexBuffer*) = 0;
		virtual void SetTexture(UINT, Texture*) = 0;
		virtual void UnsetTexture(UINT) = 0;
		virtual void SetTextureColorBuffer(UINT, RenderTarget*, UINT targetNum) = 0;
		virtual void SetTextureDepthBuffer(UINT, RenderTarget*) = 0;
		virtual void SetShader(ShaderProgram*) = 0;
		virtual void SetGpuBuffer(GPUBuffer*) = 0;
		virtual void Draw(UINT count) = 0;
		virtual void DrawIndexed(UINT count) = 0;
		virtual void SetViewport(int w0, int y0, int w, int y) = 0;
		virtual void SetRenderTarget(RenderTarget* renderTarget) = 0;
		virtual void ClearRenderTarget(RenderTarget* renderTarget, const float* color) = 0;
		virtual void SetDepthTestFunction(DEPTH_TEST_FUNCTION func) = 0;
		virtual void SetCullMode(CULL_MODE mode) = 0;
		virtual void Present() = 0;
		virtual void SetLayout(InputLayout* layout) = 0;
		virtual void SetPipelineState(PipelineState* pipeline) = 0;
		virtual InputLayout* CreateLayout(const INPUT_LAYOUT_DESC& desc) = 0;
	};
}

#endif