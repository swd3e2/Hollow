#pragma once
#include "IEntity.h"
#include "Utils/FamilyTypeID.h"

namespace ECS {

	template<class T>
	class Entity : public IEntity
	{
		void operator delete(void*) = delete;
		void operator delete[](void*) = delete;
	public:
		static const std::size_t STATIC_ENTITY_TYPE_ID;
	public:
		virtual const std::size_t GetStaticEntityTypeID() const override { return STATIC_ENTITY_TYPE_ID; };

		Entity() {}

		virtual ~Entity() {}
	};

	template<class T>
	const std::size_t Entity<T>::STATIC_ENTITY_TYPE_ID = util::Internal::FamilyTypeID<IEntity>::Get<T>();

}