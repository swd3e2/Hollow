#pragma once

#include "Shader.h"

namespace Hollow {
	class PipelineState
	{
	public:
		Shader* vertexShader;
		Shader* pixelShader;
		Shader* geometryShader;
		Shader* hullShader;
		Shader* domainShader;
		Shader* computeShader;
	};
}