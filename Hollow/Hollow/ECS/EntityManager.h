#pragma once

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <unordered_map>

#include "Hollow/Containers/array.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Platform.h"
#include "Entity.h"

#define DEFAULT_ENTITY_CONTAINER_GROW_SIZE 2048

class HOLLOW_API EntityManager : public CModule<EntityManager>
{
private:
	class IEntityContainer {};

	template<class T>
	class EntityContainer : public IEntityContainer
	{
	public:
		Hollow::array<T, 512> entityList;
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
	EntityContainer<T>* getContainer()
	{
		size_t entityTypeId = T::staticGetTypeId();

		// Trying to find container, if found - just return it
		if (entityContainers.find(entityTypeId) != entityContainers.end())
		{
			return (EntityContainer<T>*)(entityContainers[entityTypeId]);
		}
		// if not - need to create one
		EntityContainer<T>* container = new EntityContainer<T>();
		entityContainers[entityTypeId] = container;

		return container;
	}

	template<class T, typename ...ARGS>
	T* createEntity(ARGS&& ...args)
	{
		EntityContainer<T>* container = getContainer<T>();
		T* entity = container->entityList.createObject(std::forward<ARGS>(args)...);
		entity->entityId = getNextEntityId();
		
		return entity;
	}

	void startUp() { setStartedUp(); }
	void shutdown() { setShutdown(); }

	size_t getNextEntityId()
	{
		return entityIdCounter++;
	}

	template<class E>
	typename Hollow::array<E>::iterator& begin()
	{
		return getContainer<E>()->entityList.begin();
	}

	template<class E>
	typename Hollow::array<E>::iterator& end()
	{
		return getContainer<E>()->entityList.end();
	}
};


#endif