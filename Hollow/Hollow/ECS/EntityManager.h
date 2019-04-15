
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
#pragma once
#include <unordered_map>

#include "Hollow/Containers/array.h"
#include "Hollow/Core/CModule.h"
#include "Entity.h"

#define DEFAULT_ENTITY_CONTAINER_GROW_SIZE 2048

class EntityManager : public CModule<EntityManager>
{
private:
	class IEntityContainer
	{

	};
	template<class T>
	class EntityContainer
	{
	public:
		Hollow::array<Entity<T>> entityList;
		template<class ...ARGS>
		Entity<T> createEntity(ARGS&& ...args)
		{
			return entityList.createObject(std::move(args)...);
		}
	};
private:
	std::unordered_map<size_t, IEntityContainer*> entityContainers;
	size_t entityIdCounter;
public:
	EntityManager()
	{
		entityIdCounter = 0;
	}

	template<class T>
	EntityContainer<T> getEntityContainer()
	{
		size_t entityTypeId = T::getTypeId();

		// Trying to find container, if found - just return it
		if (entityContainers.find(entityTypeId) != entityContainers.end())
		{
			return entityContainers[entityTypeId];
		}

		// if not - need to create one
		EntityContainer<T>* container = new EntityContainer<T>();
		entityContainers[entityTypeId] = container;

		return container;
	}

	template<class T, typename ...ARGS>
	Entity<T>* createEntity(ARGS&& ...args)
	{
		EntityContainer<T> container = getEntityContainer<T>();
		Entity<T>* entity = container.createEntity(std::forward(args)...);
		entity->entityId = getNextEntityId();

		return entity;
	}

	void startUp() { setStartedUp(); }
	void shutdown() { setShutdown(); }

	size_t getNextEntityId()
	{
		return entityIdCounter++;
	}
};


#endif