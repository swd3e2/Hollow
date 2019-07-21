#pragma once

#ifndef HW_D3D11_BLEND_STATE_H
#define HW_D3D11_BLEND_STATE_H

#include "D3D11Prerequisites.h"

namespace Hollow {
	class D3D11BlendState
	{
	private:
		ID3D11BlendState* m_BlendState;
	public:
		D3D11BlendState();
		~D3D11BlendState();

		inline ID3D11BlendState* getBlendState() { return m_BlendState; }
	};
}

#endif