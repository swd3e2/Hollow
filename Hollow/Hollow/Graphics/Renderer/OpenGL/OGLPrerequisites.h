#pragma once

#ifndef OGL_LIBS
#define OGL_LIBS
	#pragma comment ( lib, "opengl32.lib" )
	#pragma comment ( lib, "glu32.lib" )
#endif

#define GLEW_STATIC
#include "GL/glew.h"
#include "Hollow/Common/Log.h"

class OGLWindowManager;