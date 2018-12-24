#pragma once
#include "Hollow/Utils/FamilyTypeID.h"
#include "Hollow/Platform.h"
#include "IEntity.h"

namespace Hollow {

	template <class E>
	class Entity : public IEntity
	{
	public:
		static const size_t STATIC_ENTITY_TYPE_ID;
	public:
		Entity() {}
		~Entity() {}
		virtual inline const EntityTypeID GetStaticEntityTypeID() const override
		{
			return EntityTypeID();
		}
	};

	template<class E>
	const size_t Entity<E>::STATIC_ENTITY_TYPE_ID = Core::Utils::FamilyTypeID<IEntity>::Get<E>();
}