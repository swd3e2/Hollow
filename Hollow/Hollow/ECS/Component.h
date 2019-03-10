#pragma once
#include "Hollow/Utils/FamilyTypeID.h"
#include "Hollow/Platform.h"
#include "IComponent.h"
#include "Hollow/Common/Log.h"

namespace Hollow {

	template<class T>
	class Component : public IComponent
	{
	public:
		static const size_t STATIC_COMPONENT_TYPE_ID;

		Component() { HW_DEBUG("==================>Craeted component with static type id {}<===========", STATIC_COMPONENT_TYPE_ID); }

		virtual ~Component() {}

		inline size_t GetStaticComponentTypeID() const { return STATIC_COMPONENT_TYPE_ID; }
	};

	template<class T>
	const size_t Component<T>::STATIC_COMPONENT_TYPE_ID = Core::Utils::FamilyTypeID<IComponent>::Get<T>();
}