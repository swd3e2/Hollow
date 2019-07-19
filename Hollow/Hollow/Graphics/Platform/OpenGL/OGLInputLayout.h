#pragma once

#include "Hollow/Graphics/Base/InputLayout.h"
#include "OGLPrerequisites.h"

namespace Hollow {
	class OGLInputLayout : public InputLayout
	{
	public:
		friend class OGLRenderApi;
		size_t size;
		GLuint vao;
	};
}