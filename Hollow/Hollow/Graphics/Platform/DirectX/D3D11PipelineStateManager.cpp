#include "D3D11PipelineStateManager.h"

namespace Hollow {
	PipelineState* D3D11PipelineStateManager::create(const PIPELINE_STATE_DESC& desc)
	{
		D3D11PipelineState* pipeline = new D3D11PipelineState();

		pipeline->vertexShader = desc.vertexShader;
		pipeline->pixelShader = desc.pixelShader;
		pipeline->geometryShader = desc.geometryShader;
		pipeline->domainShader = desc.domainShader;
		pipeline->hullShader = desc.hullShader;
		pipeline->computeShader = desc.computeShader;

		return pipeline;
	}
}

