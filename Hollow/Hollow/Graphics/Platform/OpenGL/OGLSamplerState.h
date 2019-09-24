#pragma once

#include "OGLPrerequisites.h"
#include "Hollow/Graphics/Base/SamplerState.h"

namespace Hollow {
	class OGLSamplerState : public SamplerState
	{
	public:
		GLuint samplerObjectId;
	};
}