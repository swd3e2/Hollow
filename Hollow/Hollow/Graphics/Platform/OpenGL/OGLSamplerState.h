#pragma once

#include "OGLPrerequisites.h"
#include "Hollow/Graphics/SamplerState.h"

namespace Hollow {
	class OGLSamplerState : public SamplerState
	{
	public:
		GLuint samplerObjectId;
	};
}