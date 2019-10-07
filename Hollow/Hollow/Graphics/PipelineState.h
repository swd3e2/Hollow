#pragma once

#include "Shader.h"
#include "SamplerState.h"
#include "DepthStencil.h"

namespace Hollow {
	struct PIPELINE_STATE_DESC
	{
		Shader* vertexShader;
		Shader* pixelShader;
		Shader* geometryShader;
		Shader* hullShader;
		Shader* domainShader;
	};


	class PipelineState
	{
	public:
		Shader* vertexShader;
		Shader* pixelShader;
		Shader* geometryShader;
		Shader* hullShader;
		Shader* domainShader;
	public:
		static PipelineState* create(const PIPELINE_STATE_DESC& desc);
	};
}