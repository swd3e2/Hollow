#pragma once

#include "Hollow/Graphics/Base/InputLayout.h"
#include "OGLPrerequisites.h"

namespace Hollow {
	class OGLInputLayout : public InputLayout
	{
	private:
		friend class OGLRenderApi;
		GLuint vao;
	};
}