#pragma once

#include "Hollow/Graphics/ShaderPipeline.h"
#include "OGLPrerequisites.h"

namespace Hollow {
	class OGLShaderPipeline : public ShaderPipeline
	{
	public:
		GLuint pipelineId;
	};
}