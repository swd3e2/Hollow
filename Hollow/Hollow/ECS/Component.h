#pragma once

#ifndef HW_COMPONENT_H
#define HW_COMPONENT_H

#include "IComponent.h"

namespace Hollow {
	template<class T>
	class Component : public IComponent
	{
	private:
		static size_t componentTypeId;
	public:
		virtual size_t getTypeId() override { return componentTypeId; }
		static size_t staticGetTypeId() { return componentTypeId; }
	};

	template<class T> size_t Component<T>::componentTypeId = typeid(T).hash_code();
}

#endif