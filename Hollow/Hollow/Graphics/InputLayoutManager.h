#pragma once

#include "Hollow/Core/CModule.h"
#include "Base/InputLayout.h"

namespace Hollow {
	class InputLayoutManager : public CModule<InputLayoutManager>
	{
	public:
		virtual InputLayout* create(const INPUT_LAYOUT_DESC& desc) = 0;
	};
}