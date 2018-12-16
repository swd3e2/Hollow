#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "Utils/FamilyTypeID.h"
#include "IEntity.h"
#include "Platform.h"

namespace Hollow {

	template <class E>
	class HOLLOW_API Entity : public IEntity
	{
	public:
		static const size_t STATIC_ENTITY_TYPE_ID;
	public:
		Entity() {}
		~Entity() {}
		virtual const EntityTypeID GetStaticEntityTypeID() const override { return this->STATIC_ENTITY_TYPE_ID; }
	};
}

#endif