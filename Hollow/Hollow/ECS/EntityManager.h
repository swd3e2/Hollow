#pragma once

#ifndef HW_ENTITY_MANAGER_H
#define HW_ENTITY_MANAGER_H

#include <unordered_map>

#include "Hollow/Core/CModule.h"
#include "Hollow/Platform.h"
#include "Entity.h"
#include "Hollow/Memory/MemoryContainer.h"
#include <unordered_map>

namespace Hollow {
	/**
	 * Entity manager
	 */
	class EntityManager : public CModule<EntityManager>
	{
	private:
		class IEntityContainer {
		public:
			virtual ~IEntityContainer() {}
		};

		template<class T>
		class EntityContainer : public IEntityContainer
		{
		public:
			MemoryContainer<T> container;
		public:
			EntityContainer() {}
			virtual ~EntityContainer() {}
		};
	private:
		std::unordered_map<size_t, IEntityContainer*> entityContainers;
		std::unordered_map<size_t, IEntity*> entities;

		size_t entityIdCounter;
	public:
		EntityManager() : entityIdCounter(0) {}

		~EntityManager()
		{
			clear();
		}

		void clear()
		{
			for (auto& container : entityContainers) {
				delete container.second;
			}
			entityContainers.clear();
		}

		template<class T, typename ...ARGS>
		T* create(ARGS&& ...args)
		{
			EntityContainer<T>* container = getContainer<T>();
			T* entity = new (container->container.allocate()) T(std::forward<ARGS>(args)...);
			entity->entityId = getNextEntityId();
			entities[entity->entityId] = entity;

			return entity;
		}

		template<class T>
		void destroy(size_t entityId)
		{
			size_t typeId = T::staticGetTypeId();

			EntityContainer<T>* container = getContainer<T>();
			container->container.deallocate((T*)entities[typeId]);
		}

		template<class E>
		typename MemoryContainer<E>::iterator& begin()
		{
			return getContainer<E>()->container.begin();
		}

		template<class E>
		typename MemoryContainer<E>::iterator& end()
		{
			return getContainer<E>()->container.end();
		}

	private:
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

		size_t getNextEntityId()
		{
			return entityIdCounter++;
		}
	};
}

#endif