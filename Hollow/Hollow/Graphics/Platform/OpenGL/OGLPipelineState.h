#pragma once

#include "Hollow/Graphics/PipelineState.h"
#include "OGLPrerequisites.h"

namespace Hollow {
	class OGLPipelineState : public PipelineState
	{
	public:
		GLuint pipelineId;
	};
}