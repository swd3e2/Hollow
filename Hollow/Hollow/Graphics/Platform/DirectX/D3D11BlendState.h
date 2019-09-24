#pragma once

#ifndef HW_D3D11_BLEND_STATE_H
#define HW_D3D11_BLEND_STATE_H

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/Base/BlendState.h"

namespace Hollow {
	class D3D11BlendState : public BlendState
	{
	private:
		friend class D3D11RenderStateManager;
		ID3D11BlendState* m_BlendState;
	public:
		~D3D11BlendState()
		{
			SAFE_RELEASE(m_BlendState);
		}

		inline ID3D11BlendState* getBlendState() { return m_BlendState; }
	};
}

#endif