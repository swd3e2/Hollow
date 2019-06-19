#include "OGLRenderTarget.h"

namespace Hollow {
	Vector4 OGLRenderTarget::readPixel(int x, int y)
	{
		// Wait until all the pending drawing commands are really done.
		// Ultra-mega-over slow ! 
		// There are usually a long time between glDrawElements() and
		// all the fragments completely rasterized.
		glFlush();
		glFinish();

		GLint drawFboId = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &drawFboId);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, this->FBO);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Read the pixel at the center of the screen.
		// You can also use glfwGetMousePos().
		// Ultra-mega-over slow too, even for 1 pixel, 
		// because the framebuffer is on the GPU.
		unsigned char data[4];
		glReadPixels(x, 900 - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, drawFboId);

		return Vector4(data[0], data[1], data[2], data[3]);
	}
}