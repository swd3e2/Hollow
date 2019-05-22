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

class OGLRenderApi : public RenderApi
{
private:
	HWND* hwnd;
	OGLWindowManager* windowManager;
public:
	OGLHardwareBufferManager*	hardwareBufferManager;
	OGLTextureManager*			textureManager;
	OGLShaderManager*			shaderManager;
public:
	OGLRenderApi(int width, int height);

	virtual void startUp() override;
	virtual void SetIndexBuffer(IndexBuffer*) override;
	virtual void SetVertexBuffer(VertexBuffer*) override;
	virtual void SetTexture(UINT, Texture*) override {}
	virtual void SetTexture(UINT, RenderTarget*) override {}
	virtual void SetShader(ShaderProgram*) override;

	void clear();
	
	virtual void Draw(UINT count)override {}
	virtual void DrawIndexed(UINT count) override;
	virtual void Present() override;
};

#endif