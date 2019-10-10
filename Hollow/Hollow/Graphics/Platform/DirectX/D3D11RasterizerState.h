#pragma once

#ifndef HW_D3D11_RASTERIZER_STATE_H
#define HW_D3D11_RASTERIZER_STATE_H

#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/RasterizerState.h"

namespace Hollow {
	class D3D11RasterizerState : public RasterizerState
	{
	private:
		friend class D3D11RenderStateManager;
		ID3D11RasterizerState* m_RasterizerState;
	public:
		~D3D11RasterizerState()
		{
			SAFE_RELEASE(m_RasterizerState);
		}

		ID3D11RasterizerState* getRasterizerState() const { return m_RasterizerState; }
	};
}

#endif
