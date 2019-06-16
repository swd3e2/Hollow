#include "D3D11SamplerState.h"
#include "D3D11RenderApi.h"
#include "D3D11Context.h"

namespace Hollow {
	D3D11SamplerState::D3D11SamplerState(D3D11_TEXTURE_ADDRESS_MODE mode)
	{
		HRESULT hr = S_OK;

		D3D11RenderApi* r = static_cast<D3D11RenderApi*>(RenderApi::instance());
		ID3D11Device* device = r->getContext().getDevice();

		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = mode; // D3D11_TEXTURE_ADDRESS_WRAP || D3D11_TEXTURE_ADDRESS_CLAMP
		samplerDesc.AddressV = mode;
		samplerDesc.AddressW = mode;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = device->CreateSamplerState(&samplerDesc, &m_SamplerState);

		if (hr != S_OK) {
			//HW_ERROR("RenderSystem: Cant create sampler state!");
		}
	}

	D3D11SamplerState::~D3D11SamplerState()
	{
		SAFE_RELEASE(m_SamplerState);
	}
}