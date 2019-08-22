#pragma once

#ifndef HW_D3D11_SAMPLER_STATE_H
#define HW_D3D11_SAMPLER_STATE_H

#include "Hollow/Graphics/Base/SamplerState.h"
#include "Hollow/Platform.h"
#include "D3D11Prerequisites.h"

namespace Hollow {
	class D3D11SamplerState : public SamplerState
	{
	private:
		friend class D3D11PipelineStateManager;
		ID3D11SamplerState* m_SamplerState;
	public:
		D3D11SamplerState() = default;
		D3D11SamplerState(D3D11_TEXTURE_ADDRESS_MODE mode);
		~D3D11SamplerState();

		inline ID3D11SamplerState* const * getSamplerState() const { return &m_SamplerState; }
	};
}

#endif