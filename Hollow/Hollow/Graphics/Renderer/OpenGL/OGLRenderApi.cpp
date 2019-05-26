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
	gpuBufferManager = new OGLGPUBufferManager();
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

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
}

void OGLRenderApi::SetTexture(UINT location, Texture* texture)
{
	OGLTexture* oglTexture = static_cast<OGLTexture*>(texture);
	glActiveTexture(location + 0x84C0);
	glBindTexture(GL_TEXTURE_2D, oglTexture->textureId);
}

void OGLRenderApi::SetShader(ShaderProgram* shader)
{
	glUseProgram(static_cast<OGLShaderProgram*>(shader)->shaderId);
}

void OGLRenderApi::SetGpuBuffer(GPUBuffer* buffer)
{
	OGLGpuBuffer* gpuBuffer = static_cast<OGLGpuBuffer*>(buffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, gpuBuffer->getLocation(), gpuBuffer->UBO);

}

void OGLRenderApi::SetViewport(int w0, int y0, int w, int y)
{
	glViewport(w0, y0, w, y);
}

void OGLRenderApi::ClearRenderTarget(RenderTarget* renderTarget, float* color)
{
}

void OGLRenderApi::clear()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
