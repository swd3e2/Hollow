#pragma once

#include "Hollow/Graphics/DepthStencil.h"
#include "D3D11Prerequisites.h"
#include "Hollow/Platform.h"

namespace Hollow {
	class D3D11DepthStencil : public DepthStencil
	{
	public:
		ID3D11DepthStencilState* depthStencilState;
	public:
		D3D11DepthStencil() = default;
		~D3D11DepthStencil()
		{
			SAFE_RELEASE(depthStencilState);
		}
	};
}