#pragma once

#include "Hollow/Graphics/InputLayout.h"
#include "Hollow/Graphics/Platform/DirectX/D3D11Prerequisites.h"

namespace Hollow {
	class D3D11InputLayout : public InputLayout
	{
	public:
		ID3D11InputLayout* m_InputLayout;
	public:
		virtual ~D3D11InputLayout() { SAFE_RELEASE(m_InputLayout); }
	};
}