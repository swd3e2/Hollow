#pragma once

#include "Hollow/Graphics/InputLayoutManager.h"
#include "D3D11InputLayout.h"

namespace Hollow {
	class D3D11InputLayoutManager : public InputLayoutManager
	{
	public:
		virtual s_ptr<InputLayout> create(const INPUT_LAYOUT_DESC& desc) override;
	};
}