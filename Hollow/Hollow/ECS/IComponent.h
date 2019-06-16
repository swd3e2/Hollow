#pragma once

#ifndef HW_I_COMPONENT_H
#define HW_I_COMPONENT_H

namespace Hollow {
	class ComponentManager;

	class IComponent
	{
		friend ComponentManager;
	protected:
		size_t componentId;
	};
}

#endif