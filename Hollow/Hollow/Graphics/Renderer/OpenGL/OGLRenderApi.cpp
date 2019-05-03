#include "OGLRenderApi.h"
#include "OGLWindowManager.h"

OGLRenderApi::OGLRenderApi(int width, int height)
{
	windowManager = new OGLWindowManager();
}

void OGLRenderApi::startUp()
{
	hwnd = static_cast<OGLWin32Window*>(windowManager->getWindow())->getHWND();
}
