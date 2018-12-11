#ifndef __COMPONENT__
#define __COMPONENT__
#pragma once

#include "Engine/Utils/FamilyTypeID.h"
#include "IComponent.h"

namespace Hollow {

	template<class T>
	class Component : public IComponent
	{
	public:
		static const unsigned int STATIC_COMPONENT_TYPE_ID;

		Component() {}

		virtual ~Component() {}

		inline unsigned int GetStaticComponentTypeID() const { return STATIC_COMPONENT_TYPE_ID; }
	};

	template<class T>
	const unsigned int Component<T>::STATIC_COMPONENT_TYPE_ID = Core::Utils::FamilyTypeID<IComponent>::Get<T>();
}

#endif