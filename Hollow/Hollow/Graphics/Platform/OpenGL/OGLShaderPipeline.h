#pragma once

#include "Hollow/Graphics/ShaderPipeline.h"
#include "OGLPrerequisites.h"

namespace Hollow {
	class OGLShaderPipeline : public ShaderPipeline
	{
	private:
		friend class OGLShaderManager;
	public:
		GLuint pipelineId;
	};
}