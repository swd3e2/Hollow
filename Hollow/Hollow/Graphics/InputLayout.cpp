#include "InputLayout.h"
#include "Hollow/Graphics/InputLayoutManager.h"

namespace Hollow {
	s_ptr<InputLayout> InputLayout::create(const INPUT_LAYOUT_DESC& desc)
	{
		return InputLayoutManager::instance()->create(desc);
	}
}
