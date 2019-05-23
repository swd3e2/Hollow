#include "OGLRenderApi.h"
#include "OGLWindowManager.h"

OGLRenderApi::OGLRenderApi(int width, int height)
{
	windowManager = new OGLWindowManager();
}

void OGLRenderApi::startUp()
{
	hardwareBufferManager = new OGLHardwareBufferManager();
	textureManager = new OGLTextureManager();
	shaderManager = new OGLShaderManager();
	hwnd = static_cast<OGLWin32Window*>(windowManager->getWindow())->getHWND();
}

void OGLRenderApi::SetIndexBuffer(IndexBuffer* buffer)
{
	OGLIndexBuffer* iBuffer = static_cast<OGLIndexBuffer*>(buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer->IBO);
}

void OGLRenderApi::SetVertexBuffer(VertexBuffer* buffer)
{
	glBindVertexArray(static_cast<OGLVertexBuffer*>(buffer)->VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, static_cast<OGLVertexBuffer*>(buffer)->VBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
}

void OGLRenderApi::SetShader(ShaderProgram* shader)
{
	glUseProgram(static_cast<OGLShaderProgram*>(shader)->shaderId);
}

void OGLRenderApi::clear()
{
	glViewport(0, 0, 2560, 1440);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRenderApi::DrawIndexed(UINT count)
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

void OGLRenderApi::Present()
{
	SwapBuffers(GetDC(*hwnd));
}
