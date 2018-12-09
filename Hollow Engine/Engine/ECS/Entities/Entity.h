#pragma once

#ifndef __ENTITY__
#define __ENTITY__
#include "../FamilyTypeID.h"
#include "IEntity.h"

namespace Hollow {

	template <class E>
	class Entity : public IEntity
	{
	public:
		static const unsigned int STATIC_ENTITY_TYPE_ID;
	public:
		Entity() {}
		~Entity() {}
		virtual const EntityTypeID GetStaticEntityTypeID() const override { return this->STATIC_ENTITY_TYPE_ID; }
	};

	template<class E>
	const unsigned int Entity<E>::STATIC_ENTITY_TYPE_ID = Core::Utils::FamilyTypeID<IEntity>::Get<E>();
}

#endif