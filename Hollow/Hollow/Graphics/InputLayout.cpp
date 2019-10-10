#include "InputLayout.h"
#include "Hollow/Graphics/InputLayoutManager.h"

namespace Hollow {
	InputLayout* InputLayout::create(const INPUT_LAYOUT_DESC& desc)
	{
		return InputLayoutManager::instance()->create(desc);
	}
}
