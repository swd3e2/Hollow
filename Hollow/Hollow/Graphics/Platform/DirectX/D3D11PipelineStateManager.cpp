#include "D3D11PipelineStateManager.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

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

	SamplerState* D3D11PipelineStateManager::create(const SAMPLER_STATE_DESC& desc)
	{
		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11Device* device = r->getContext().getDevice();

		D3D11SamplerState* samplerState = new D3D11SamplerState();
		
		D3D11_SAMPLER_DESC samplerDesc = {};

		samplerDesc.AddressU = D3D11Helper::getAddressingMode(desc.addressingMode);
		samplerDesc.AddressV = D3D11Helper::getAddressingMode(desc.addressingMode);
		samplerDesc.AddressW = D3D11Helper::getAddressingMode(desc.addressingMode);
		samplerDesc.ComparisonFunc = D3D11Helper::getComparisonFunction(desc.comparisonFunction);
		samplerDesc.MinLOD = desc.minLOD;
		samplerDesc.MaxLOD = desc.maxLOD;
		samplerDesc.MipLODBias = desc.mipLODBias;
		samplerDesc.Filter = D3D11Helper::getFilteringMode(desc.minFilterModel, desc.magFilterMode, desc.mipFilterMode, desc.comparisonFunction != ComparisonFunction::NEVER);

		device->CreateSamplerState(&samplerDesc, &samplerState->m_SamplerState);

		return samplerState;
	}
}