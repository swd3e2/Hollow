#pragma once

#ifndef HW_D3D11_SAMPLER_STATE_H
#define HW_D3D11_SAMPLER_STATE_H

#include "Hollow/Graphics/SamplerState.h"
#include "Hollow/Platform.h"
#include "D3D11Prerequisites.h"

namespace Hollow {
	class D3D11SamplerState : public SamplerState
	{
	private:
		friend class D3D11PipelineStateManager;
		friend class D3D11RenderStateManager;
		ID3D11SamplerState* m_SamplerState;
	public:
		~D3D11SamplerState()
		{
			SAFE_RELEASE(m_SamplerState);
		}

		inline ID3D11SamplerState** getSamplerState() { return &m_SamplerState; }
	};
}

#endif