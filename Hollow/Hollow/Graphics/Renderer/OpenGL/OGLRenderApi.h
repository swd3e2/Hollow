#pragma once

#ifndef HW_OGL_RENDER_API_H
#define HW_OGL_RENDER_API_H

#include "OGLPrerequisites.h"
#include "Hollow/Graphics/Renderer/Base/RenderApi.h"
#include <iostream>

class HOLLOW_API OGLRenderApi : public RenderApi
{
private:
	HWND* hwnd;
	OGLWindowManager* windowManager;
public:
	OGLRenderApi(int width, int height);

	virtual void startUp() override;
	virtual void SetIndexBuffer(IndexBuffer*) override {}
	virtual void SetVertexBuffer(VertexBuffer*) override {}
	virtual void SetTexture(UINT, Texture*) override {}
	virtual void SetTexture(UINT, RenderTarget*) override {}
	virtual void SetShader(Shader*) override {}

	void clear() 
	{
		glViewport(0, 0, 2560, 1440);
		glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SwapBuffers(GetDC(*hwnd));
	}
};

#endif