#pragma once

#include "Hollow/Core/CModule.h"
#include "InputLayout.h"

namespace Hollow {
	class InputLayoutManager : public CModule<InputLayoutManager>
	{
	public:
		virtual s_ptr<InputLayout> create(const INPUT_LAYOUT_DESC& desc) = 0;
	};
}