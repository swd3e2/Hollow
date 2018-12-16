#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "Utils/FamilyTypeID.h"
#include "IComponent.h"
#include "Platform.h"

namespace Hollow {

	template<class T>
	class HOLLOW_API Component : public IComponent
	{
	public:
		static const size_t STATIC_COMPONENT_TYPE_ID;

		Component() {}

		virtual ~Component() {}

		inline size_t GetStaticComponentTypeID() const { return STATIC_COMPONENT_TYPE_ID; }
	};


}

#endif