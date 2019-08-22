#pragma once

#include "OGLPrerequisites.h"
#include "Hollow/Graphics/PipelineStateManager.h"
#include "OGLPipelineState.h"
#include "OGLShader.h"
#include "OGLSamplerState.h"
#include "OGLHelper.h"

namespace Hollow {
	class OGLPipelineStateManager : PipelineStateManager
	{
		virtual PipelineState* create(const PIPELINE_STATE_DESC& desc) override;
		virtual SamplerState* create(const SAMPLER_STATE_DESC& desc) override;
	};
}