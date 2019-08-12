#pragma once

#ifndef HW_ENTITY_H
#define HW_ENTITY_H

#include "IEntity.h"

namespace Hollow {
	class EntityManager;

	template<class T>
	class Entity : public IEntity
	{
		friend class EntityManager;
	private:
		static size_t entityTypeId;
	public:
		Entity() {}
		static size_t staticGetTypeId() { return entityTypeId; }
		virtual size_t getTypeId() override { return entityTypeId; }
	};

	template<class T> size_t Entity<T>::entityTypeId = typeid(Entity<T>).hash_code();
}

#endif