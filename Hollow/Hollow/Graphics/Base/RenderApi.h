#pragma once

#ifndef HW_RENDER_API_H
#define HW_RENDER_API_H

#include "Hollow/Platform.h"
#include "Hollow/Core/CModule.h"
#include "HardwareBuffer.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "Shader.h"
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
		virtual void setIndexBuffer(IndexBuffer*) = 0;
		virtual void setVertexBuffer(VertexBuffer*) = 0;
		virtual void setTexture(UINT, Texture*) = 0;
		virtual void unsetTexture(UINT) = 0;
		virtual void setTextureColorBuffer(UINT, RenderTarget*, UINT targetNum) = 0;
		virtual void setTextureDepthBuffer(UINT, RenderTarget*) = 0;
		virtual void setGpuBuffer(GPUBuffer*) = 0;
		virtual void draw(UINT count) = 0;
		virtual void drawIndexed(UINT count) = 0;
		virtual void setViewport(int w0, int y0, int w, int y) = 0;
		virtual void setRenderTarget(RenderTarget* renderTarget) = 0;
		virtual void clearRenderTarget(RenderTarget* renderTarget, const float* color) = 0;
		virtual void setDepthTestFunction(DEPTH_TEST_FUNCTION func) = 0;
		virtual void setCullMode(CULL_MODE mode) = 0;
		virtual void present() = 0;
		virtual void setInputLayout(InputLayout* layout) = 0;
		virtual void setPipelineState(PipelineState* pipeline) = 0;
	};
}

#endif