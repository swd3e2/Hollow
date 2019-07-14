#pragma once

#include "Hollow/Graphics/Base/PipelineState.h"
#include "OGLPrerequisites.h"

namespace Hollow {
	class OGLPipelineState : public PipelineState
	{
	public:
		GLuint pipelineId;
	};
}