#pragma once
#include <d3d11.h>
#include "Hollow/Common/Log.h"
#include "Hollow/Platform.h"

class D3DSamplerState
{
private:
	ID3D11SamplerState* m_SamplerState;
public:
	D3DSamplerState(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE mode)
	{
		HRESULT hr = S_OK;

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
			HW_ERROR("RenderSystem: Cant create sampler state!");
		}
	}

	~D3DSamplerState()
	{
		SAFE_RELEASE(m_SamplerState);
	}

	ID3D11SamplerState* const * GetSamplerState()
	{
		return &m_SamplerState;
	}
};