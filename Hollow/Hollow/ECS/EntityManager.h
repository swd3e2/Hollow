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
			virtual void destroy(void* mem) = 0;
		};

		template<class T>
		class EntityContainer : public IEntityContainer
		{
		public:
			MemoryContainer<T> container;
		public:
			EntityContainer() {}
			virtual ~EntityContainer() {}

			virtual void destroy(void* mem) override
			{
				container.deallocate(mem);
			}
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

		void destroy(size_t entityId)
		{
			IEntityContainer* container = entityContainers[entities[entityId]->getTypeId()];
			container->destroy(entities[entityId]);
			ComponentManager::instance()->destroyAll(entityId);
		}

		IEntity* get(size_t entityId)
		{
			return entities[entityId];
		}

		template<class E>
		typename MemoryContainer<E>& container()
		{
			return getContainer<E>()->container;
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