#pragma once

#ifndef HW_D3D11_SAMPLER_STATE_H
#define HW_D3D11_SAMPLER_STATE_H

#include "Hollow/Graphics/Renderer/Base/SamplerState.h"
#include "Hollow/Platform.h"
#include "D3D11Prerequisites.h"

class  D3D11SamplerState : public SamplerState
{
private:
	ID3D11SamplerState* m_SamplerState;
public:
	D3D11SamplerState(D3D11_TEXTURE_ADDRESS_MODE mode);
	~D3D11SamplerState();

	inline ID3D11SamplerState* const* GetSamplerState() { return &m_SamplerState; }
};

#endif